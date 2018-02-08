#include <primitiv/config.h>

#include <primitiv/cuda16_device.h>
#include <primitiv/cuda_utils.h>
#include <primitiv/device_ops/cuda16/common.h>

namespace primitiv {
namespace devices {

void CUDA16::reset_tensor_by_array_impl(const float values[], Tensor &x) {
  const std::uint32_t size = x.shape().size();
  CUDA_CALL(::cudaSetDevice(dev_id_));
  CUDA_CALL(::cudaMemcpy(
        MDATA(x), values, sizeof(float) * size, cudaMemcpyHostToDevice));
}

}  // namespace devices
}  // namespace primitiv
