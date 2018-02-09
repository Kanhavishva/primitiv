#include <primitiv/config.h>

#include <primitiv/cuda16_device.h>
#include <primitiv/cuda_utils.h>
#include <primitiv/device_ops/cuda16/common.h>

namespace {
/*
__global__ void rand_affine_dev(
    float shift, float scale, std::uint32_t size, float *py) {
  const std::uint32_t i = IDX;
  if (i < size) py[i] = py[i] * scale + shift;
}
*/
}  // namespace

namespace primitiv {
namespace devices {

void CUDA16::random_uniform_impl(float lower, float upper, Tensor &y) {
  THROW_NOT_IMPLEMENTED;
  /*const std::uint32_t size = y.shape().size();
  const std::uint32_t num_blocks = GRID_SIZE(size, dim1_x_);
  const float scale = upper - lower;
  CUDA_CALL(::cudaSetDevice(dev_id_));
  CURAND_CALL(::curandGenerateUniform(state_->curand.get(), MDATA(float, y), size));
  ::rand_affine_dev<<<num_blocks, dim1_x_>>>(lower, scale, size, MDATA(float, y));
*/}

}  // namespace devices
}  // namespace primitiv
