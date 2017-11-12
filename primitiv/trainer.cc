#include <config.h>

#include <cmath>
#include <fstream>
#include <primitiv/error.h>
#include <primitiv/file_format.h>
#include <primitiv/model.h>
#include <primitiv/msgpack/reader.h>
#include <primitiv/msgpack/writer.h>
#include <primitiv/operators.h>
#include <primitiv/parameter.h>
#include <primitiv/trainer.h>

namespace primitiv {

void Trainer::load(const std::string &path) {
  std::ifstream ifs(path);
  if (!ifs.is_open()) {
    THROW_ERROR("Could not open file: " << path);
  }
  msgpack::Reader reader(ifs);

  std::uint32_t major, minor;
  reader >> major >> minor;
  FileFormat::check_version(major, minor);

  std::uint32_t datatype;
  reader >> datatype;
  FileFormat::check_datatype(FileFormat::DataType::TRAINER, datatype);

  std::unordered_map<std::string, std::uint32_t> uint_configs;
  std::unordered_map<std::string, float> float_configs;
  reader >> uint_configs >> float_configs;
  set_configs(uint_configs, float_configs);
}

void Trainer::save(const std::string &path) const {
  std::unordered_map<std::string, std::uint32_t> uint_configs;
  std::unordered_map<std::string, float> float_configs;
  get_configs(uint_configs, float_configs);

  std::ofstream ofs(path);
  if (!ofs.is_open()) {
    THROW_ERROR("Could not open file: " << path);
  }
  msgpack::Writer writer(ofs);

  writer << FileFormat::CurrentVersion::MAJOR;
  writer << FileFormat::CurrentVersion::MINOR;
  writer << static_cast<std::uint32_t>(FileFormat::DataType::TRAINER);
  writer << uint_configs << float_configs;
}

void Trainer::add_parameter(Parameter &param) {
  if (params_.find(&param) != params_.end()) {
    THROW_ERROR("Parameter '" << &param << "' is already registered.");
  }
  params_.insert(&param);
  configure_parameter(param);
}

void Trainer::add_model(const Model &model) {
  for (Parameter *param : model.get_trainable_parameters()) {
    add_parameter(*param);
  }
}

void Trainer::reset_gradients() {
  for (Parameter *param : params_) {
    param->reset_gradient();
  }
}

void Trainer::update() {
  if (l2_strength_ > 0) {
    // Weight decay
    for (Parameter *param : params_) {
      param->gradient() += l2_strength_ * param->value();
    }
  }

  if (clip_threshold_ > 0) {
    // Gradient clipping
    float sq_norm = 0;
    for (const Parameter *param : params_) {
      const Tensor &g = param->gradient();
      sq_norm += operators::sum(operators::flatten(g * g), 0).to_float();
    }
    if (sq_norm > clip_threshold_ * clip_threshold_) {
      float clip_scale = clip_threshold_ / std::sqrt(sq_norm);
      for (Parameter *param : params_) {
        param->gradient() *= clip_scale;
      }
    }
  }

  for (Parameter *param : params_) {
    update_parameter(lr_scale_, *param);
  }

  ++epoch_;
}

void Trainer::get_configs(
    std::unordered_map<std::string, std::uint32_t> &uint_configs,
    std::unordered_map<std::string, float> &float_configs) const {
  uint_configs.insert(std::make_pair("Trainer.epoch", epoch_));
  float_configs.insert(std::make_pair("Trainer.lr_scale", lr_scale_));
  float_configs.insert(std::make_pair("Trainer.l2_strength", l2_strength_));
  float_configs.insert(std::make_pair("Trainer.clip_threshold", clip_threshold_));
}

void Trainer::set_configs(
    const std::unordered_map<std::string, std::uint32_t> &uint_configs,
    const std::unordered_map<std::string, float> &float_configs) {
#define SET_CONFIG(dest, cfg, key) { \
  const auto it = cfg.find(key); \
  if (it == cfg.end()) { \
    THROW_ERROR("Key not found in the trainer config: " << key); \
  } \
  dest = it->second; \
}
  SET_CONFIG(epoch_, uint_configs, "Trainer.epoch");
  SET_CONFIG(lr_scale_, float_configs, "Trainer.lr_scale");
  SET_CONFIG(l2_strength_, float_configs, "Trainer.l2_strength");
  SET_CONFIG(clip_threshold_, float_configs, "Trainer.clip_threshold");
#undef SET_CONFIG
}

}  // namespace primitiv
