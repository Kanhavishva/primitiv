#include <primitiv/config.h>

#include <primitiv/cuda16_device.h>
#include <primitiv/cuda_utils.h>
#include <primitiv/device_ops/cuda16/common.h>

namespace {
/*
__global__ void set_identity_dev(std::uint32_t size, std::uint32_t skip, float *py) {
  const std::uint32_t i = IDX;
  if (i < size) py[i] = !(i % skip);
}
*/
}  // namespace

namespace primitiv {
namespace devices {

void CUDA16::identity_impl(Tensor &y) {
  THROW_NOT_IMPLEMENTED;
  /*const std::uint32_t size = y.shape().size();
  const std::uint32_t skip = y.shape()[0] + 1;
  const std::uint32_t num_blocks = GRID_SIZE(size, dim1_x_);
  CUDA_CALL(::cudaSetDevice(dev_id_));
  ::set_identity_dev<<<num_blocks, dim1_x_>>>(size, skip, MDATA(float, y));
*/}

}  // namespace devices
}  // namespace primitiv
