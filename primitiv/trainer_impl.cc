#include <config.h>

#include <algorithm>
#include <cmath>
#include <primitiv/operators.h>
#include <primitiv/parameter.h>
#include <primitiv/trainer_impl.h>

namespace primitiv {
namespace trainers {

#define SET_CONFIG(dest, cfg, key) { \
  const auto it = cfg.find(key); \
  if (it == cfg.end()) { \
    THROW_ERROR("Key not found in the trainer config: " << key); \
  } \
  dest = it->second; \
}

void SGD::configure_parameter(Parameter &param) {}

void SGD::update_parameter(float scale, Parameter &param) {
  param.value() -= (scale * eta_) * param.gradient();
}

void SGD::get_configs(
    std::unordered_map<std::string, unsigned> &uint_configs,
    std::unordered_map<std::string, float> &float_configs) const {
  Trainer::get_configs(uint_configs, float_configs);
  float_configs.insert(std::make_pair("SGD.eta", eta_));
}

void SGD::set_configs(
    const std::unordered_map<std::string, unsigned> &uint_configs,
    const std::unordered_map<std::string, float> &float_configs) {
  Trainer::set_configs(uint_configs, float_configs);
  SET_CONFIG(eta_, float_configs, "SGD.eta");
}

void MomentumSGD::configure_parameter(Parameter &param) {
  const std::string name = "momentumsgd-m";
  if (!param.has_stats(name)) {
    param.add_stats(name, param.shape());
  }
}

void MomentumSGD::update_parameter(float scale, Parameter &param) {
  Tensor &m = param.stats("momentumsgd-m");
  m *= momentum_;
  m -= (scale * eta_) * param.gradient();
  param.value() += m;
}

void MomentumSGD::get_configs(
    std::unordered_map<std::string, unsigned> &uint_configs,
    std::unordered_map<std::string, float> &float_configs) const {
  Trainer::get_configs(uint_configs, float_configs);
  float_configs.insert(std::make_pair("MomentumSGD.eta", eta_));
  float_configs.insert(std::make_pair("MomentumSGD.momentum", momentum_));
}

void MomentumSGD::set_configs(
    const std::unordered_map<std::string, unsigned> &uint_configs,
    const std::unordered_map<std::string, float> &float_configs) {
  Trainer::set_configs(uint_configs, float_configs);
  SET_CONFIG(eta_, float_configs, "MomentumSGD.eta");
  SET_CONFIG(momentum_, float_configs, "MomentumSGD.momentum");
}

void AdaGrad::configure_parameter(Parameter &param) {
  const std::string name = "adagrad-m";
  if (!param.has_stats(name)) {
    param.add_stats(name, param.shape());
  }
}

void AdaGrad::update_parameter(float scale, Parameter &param) {
  const Tensor &g = param.gradient();
  Tensor &m = param.stats("adagrad-m");
  m += g * g;
  param.value() -= (scale * eta_) * g / (operators::sqrt(m) + eps_);
}

void AdaGrad::get_configs(
    std::unordered_map<std::string, unsigned> &uint_configs,
    std::unordered_map<std::string, float> &float_configs) const {
  Trainer::get_configs(uint_configs, float_configs);
  float_configs.insert(std::make_pair("AdaGrad.eta", eta_));
  float_configs.insert(std::make_pair("AdaGrad.eps", eps_));
}

void AdaGrad::set_configs(
    const std::unordered_map<std::string, unsigned> &uint_configs,
    const std::unordered_map<std::string, float> &float_configs) {
  Trainer::set_configs(uint_configs, float_configs);
  SET_CONFIG(eta_, float_configs, "AdaGrad.eta");
  SET_CONFIG(eps_, float_configs, "AdaGrad.eps");
}

void RMSProp::configure_parameter(Parameter &param) {
  const std::string name = "rmsprop-m";
  if (!param.has_stats(name)) {
    param.add_stats(name, param.shape());
  }
}

void RMSProp::update_parameter(float scale, Parameter &param) {
  const Tensor &g = param.gradient();
  Tensor &m = param.stats("rmsprop-m");
  m = alpha_ * m + (1 - alpha_) * g * g;
  param.value() -= (scale * eta_) * g / (operators::sqrt(m) + eps_);
}

void RMSProp::get_configs(
    std::unordered_map<std::string, unsigned> &uint_configs,
    std::unordered_map<std::string, float> &float_configs) const {
  Trainer::get_configs(uint_configs, float_configs);
  float_configs.insert(std::make_pair("RMSProp.eta", eta_));
  float_configs.insert(std::make_pair("RMSProp.alpha", alpha_));
  float_configs.insert(std::make_pair("RMSProp.eps", eps_));
}

void RMSProp::set_configs(
    const std::unordered_map<std::string, unsigned> &uint_configs,
    const std::unordered_map<std::string, float> &float_configs) {
  Trainer::set_configs(uint_configs, float_configs);
  SET_CONFIG(eta_, float_configs, "RMSProp.eta");
  SET_CONFIG(alpha_, float_configs, "RMSProp.alpha");
  SET_CONFIG(eps_, float_configs, "RMSProp.eps");
}

void AdaDelta::configure_parameter(Parameter &param) {
  for (const char *name : {"adadelta-m1", "adadelta-m2"}) {
    if (!param.has_stats(name)) {
      param.add_stats(name, param.shape());
    }
  }
}

void AdaDelta::update_parameter(float scale, Parameter &param) {
  const Tensor &g = param.gradient();
  Tensor &m1 = param.stats("adadelta-m1");
  Tensor &m2 = param.stats("adadelta-m2");
  m2 *= rho_;
  m2 += (1 - rho_) * g * g;
  const Tensor dx = operators::sqrt((m1 + eps_) / (m2 + eps_)) * g;
  m1 *= rho_;
  m1 += (1 - rho_) * dx * dx;
  param.value() -= scale * dx;
}

void AdaDelta::get_configs(
    std::unordered_map<std::string, unsigned> &uint_configs,
    std::unordered_map<std::string, float> &float_configs) const {
  Trainer::get_configs(uint_configs, float_configs);
  float_configs.insert(std::make_pair("AdaDelta.rho", rho_));
  float_configs.insert(std::make_pair("AdaDelta.eps", eps_));
}

void AdaDelta::set_configs(
    const std::unordered_map<std::string, unsigned> &uint_configs,
    const std::unordered_map<std::string, float> &float_configs) {
  Trainer::set_configs(uint_configs, float_configs);
  SET_CONFIG(rho_, float_configs, "AdaDelta.rho");
  SET_CONFIG(eps_, float_configs, "AdaDelta.eps");
}

void Adam::configure_parameter(Parameter &param) {
  for (const char *name : {"adam-m1", "adam-m2"}) {
    if (!param.has_stats(name)) {
      param.add_stats(name, param.shape());
    }
  }
}

void Adam::update_parameter(float scale, Parameter &param) {
  const unsigned epoch = get_epoch() + 1;
  const Tensor &g = param.gradient();
  Tensor &m1 = param.stats("adam-m1");
  Tensor &m2 = param.stats("adam-m2");
  m1 = beta1_ * m1 + (1 - beta1_) * g;
  m2 = beta2_ * m2 + (1 - beta2_) * g * g;
  const Tensor mm1 = m1 / (1 - std::pow(beta1_, epoch));
  const Tensor mm2 = m2 / (1 - std::pow(beta2_, epoch));
  param.value() -= (scale * alpha_) * mm1 / (operators::sqrt(mm2) + eps_);
}

void Adam::get_configs(
    std::unordered_map<std::string, unsigned> &uint_configs,
    std::unordered_map<std::string, float> &float_configs) const {
  Trainer::get_configs(uint_configs, float_configs);
  float_configs.insert(std::make_pair("Adam.alpha", alpha_));
  float_configs.insert(std::make_pair("Adam.beta1", beta1_));
  float_configs.insert(std::make_pair("Adam.beta2", beta2_));
  float_configs.insert(std::make_pair("Adam.eps", eps_));
}

void Adam::set_configs(
    const std::unordered_map<std::string, unsigned> &uint_configs,
    const std::unordered_map<std::string, float> &float_configs) {
  Trainer::set_configs(uint_configs, float_configs);
  SET_CONFIG(alpha_, float_configs, "Adam.alpha");
  SET_CONFIG(beta1_, float_configs, "Adam.beta1");
  SET_CONFIG(beta2_, float_configs, "Adam.beta2");
  SET_CONFIG(eps_, float_configs, "Adam.eps");
}

#undef SET_CONFIG

}  // namespace trainers
}  // namespace primitiv
