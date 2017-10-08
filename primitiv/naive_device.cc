#include <config.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <primitiv/naive_device.h>
#include <primitiv/error.h>

using std::cerr;
using std::endl;

namespace primitiv {
namespace devices {

void Naive::dump_description() const {
  cerr << "Device " << this << ':' << endl;
  cerr << "  Type: Naive" << endl;
}

std::shared_ptr<void> Naive::new_handle(const Shape &shape) {
  const unsigned mem_size = sizeof(float) * shape.size();
  void *data = std::malloc(mem_size);
  if (!data) {
    THROW_ERROR("Memory allocation failed. Requested size: " << mem_size);
  }
  return std::shared_ptr<void>(data, std::free);
}

#define DATA(x) static_cast<float *>((x).data())
#define CDATA(x) static_cast<const float *>((x).data())

#define REPEAT_OP(i, n, op) \
  for (unsigned (i) = 0; (i) < (n); ++(i)) { (op); }

std::vector<float> Naive::tensor_to_vector_impl(const Tensor &x) {
  const unsigned num_elements = x.shape().size();
  std::vector<float> ret(num_elements);
  std::memcpy(&ret[0], x.data(), sizeof(float) * num_elements);
  return ret;
}

void Naive::reset_tensor_impl(float k, Tensor &x) {
  float *dest = DATA(x);
  const unsigned size = x.shape().size();
  REPEAT_OP(i, size, dest[i] = k);
}

void Naive::reset_tensor_by_array_impl(const float values[], Tensor &x) {
  std::memcpy(x.data(), values, sizeof(float) * x.shape().size());
}

void Naive::copy_tensor_impl(const Tensor &x, Tensor &y) {
  switch (x.device().type()) {
    case Device::DEVICE_TYPE_CPU:
      reset_tensor_by_array(CDATA(x), y);
      break;
    default:
      reset_tensor_by_vector(x.to_vector(), y);
  }
}

void Naive::identity_impl(Tensor &y) {
  reset_tensor_impl(0, y);
  float *dest = DATA(y);
  const unsigned size = y.shape()[0];
  REPEAT_OP(i, size, dest[i * (size + 1)] = 1);
}

void Naive::random_bernoulli_impl(float p, Tensor &y) {
  std::bernoulli_distribution dist(p);
  float *dest = DATA(y);
  const unsigned size = y.shape().size();
  REPEAT_OP(i, size, dest[i] = dist(rng_));
}

void Naive::random_uniform_impl(float lower, float upper, Tensor &y) {
  std::uniform_real_distribution<float> dist(lower, upper);
  float *dest = DATA(y);
  const unsigned size = y.shape().size();
  for (unsigned i = 0; i < size; ++i) {
    const float x = dist(rng_);
    dest[i] = x == lower ? upper : x;
  }
}

void Naive::random_normal_impl(float mean, float sd, Tensor &y) {
  std::normal_distribution<float> dist(mean, sd);
  float *dest = DATA(y);
  const unsigned size = y.shape().size();
  REPEAT_OP(i, size, dest[i] = dist(rng_));
}

void Naive::random_log_normal_impl(float mean, float sd, Tensor &y) {
  std::lognormal_distribution<float> dist(mean, sd);
  float *dest = DATA(y);
  const unsigned size = y.shape().size();
  REPEAT_OP(i, size, dest[i] = dist(rng_));
}

void Naive::pick_fw_impl(
    const Tensor &x, const std::vector<unsigned> &ids, unsigned dim,
    Tensor &y) {
  const unsigned bs = y.shape().batch();
  const unsigned skip_x = x.shape().has_batch() * x.shape().volume();
  const unsigned skip_i = ids.size() > 1;
  const unsigned base = y.shape().lower_volume(dim);
  const unsigned skip = base * x.shape()[dim];
  const unsigned repeat = y.shape().volume() / base;

  float *dest = DATA(y);
  for (unsigned batch = 0; batch < bs; ++batch) {
    const float *src = CDATA(x) + batch * skip_x + base * ids[batch * skip_i];
    for (unsigned i = 0; i < repeat; ++i) {
      const float *sp = src;
      REPEAT_OP(j, base, *dest++ = *sp++);
      src += skip;
    }
  }
}

void Naive::slice_fw_impl(
    const Tensor &x, unsigned dim, unsigned offset, Tensor &y) {
  const unsigned base = y.shape().lower_volume(dim);
  const unsigned span = base * y.shape()[dim];
  const unsigned skip = base * x.shape()[dim];
  const unsigned repeat = y.shape().size() / span;

  float *dest = DATA(y);
  const float *src = CDATA(x) + base * offset;
  for (unsigned i = 0; i < repeat; ++i) {
    const float *sp = src;
    REPEAT_OP(j, span, *dest++ = *sp++);
    src += skip;
  }
}

void Naive::concat_fw_impl(
    const std::vector<const Tensor *> &xs, unsigned dim, Tensor &y) {
  const unsigned new_bs = y.shape().batch();
  const unsigned base = y.shape().lower_volume(dim);
  const unsigned skip = base * y.shape()[dim];
  const unsigned repeat = y.shape().volume() / skip;

  unsigned offset = 0;
  for (const Tensor *x : xs) {
    const unsigned src_dim = x->shape()[dim];
    const unsigned span = base * src_dim;
    const unsigned b_skip = x->shape().has_batch() * span * repeat;
    float *dest = DATA(y) + offset;
    const float *src = CDATA(*x);
    for (unsigned batch = 0; batch < new_bs; ++batch) {
      const float *sp = src;
      for (unsigned i = 0; i < repeat; ++i) {
        float *dp = dest;
        REPEAT_OP(j, span, *dp++ = *sp++);
        dest += skip;
      }
      src += b_skip;
    }
    offset += span;
  }
}

void Naive::pick_bw_impl(
    const Tensor &gy, const std::vector<unsigned>& ids, unsigned dim,
    Tensor &gx) {
  const unsigned bs = gy.shape().batch();
  const unsigned skip_x = gx.shape().has_batch() * gx.shape().volume();
  const unsigned skip_i = ids.size() > 1;
  const unsigned base = gy.shape().lower_volume(dim);
  const unsigned skip = base * gx.shape()[dim];
  const unsigned repeat = gy.shape().volume() / base;
  const float *src = CDATA(gy);
  for (unsigned batch = 0; batch < bs; ++batch) {
    float *dest = DATA(gx) + batch * skip_x + base * ids[batch * skip_i];
    for (unsigned i = 0; i < repeat; ++i) {
      float *dp = dest;
      REPEAT_OP(j, base, *dp++ += *src++);
      dest += skip;
    }
  }
}

void Naive::slice_bw_impl(
    const Tensor &gy, unsigned dim, unsigned offset, Tensor &gx) {
  const Shape &sy = gy.shape();
  const Shape &sx = gx.shape();
  const unsigned base = sx.lower_volume(dim);
  const unsigned span = base * sy[dim];
  const unsigned skip = base * sx[dim];
  const unsigned repeat = sx.volume() / skip;
  const unsigned bs = std::max(sx.batch(), sy.batch());
  const unsigned b_skip_d = sx.has_batch() * sx.volume();
  const unsigned b_skip_s = sy.has_batch() * sy.volume();
  float *dest = DATA(gx) + base * offset;
  const float *src = CDATA(gy);
  for (unsigned batch = 0; batch < bs; ++batch) {
    float *dp = dest;
    const float *sp = src;
    for (unsigned i = 0; i < repeat; ++i) {
      float *ddp = dp;
      REPEAT_OP(j, span, *ddp++ += *sp++);
      dp += skip;
    }
    dest += b_skip_d;
    src += b_skip_s;
  }
}

#define CPUDEV_FW_X(name, op) \
void Naive::name##_fw_impl(const Tensor &x, Tensor &y) { \
  float *dest = DATA(y); \
  const float *src = CDATA(x); \
  const unsigned size = x.shape().size(); \
  REPEAT_OP(i, size, dest[i] = (op)); \
}

#define CPUDEV_BW_X(name, op) \
void Naive::name##_bw_impl( \
    const Tensor &x, const Tensor &y, const Tensor &gy, Tensor &gx) { \
  const float *px = CDATA(x); static_cast<void>(px); \
  const float *py = CDATA(y); static_cast<void>(py); \
  const float *pgy = CDATA(gy); \
  float *pgx = DATA(gx); \
  const unsigned size = x.shape().size(); \
  REPEAT_OP(i, size, pgx[i] += (op)); \
}

#define CPUDEV_FW_X_CONST(name, op) \
void Naive::name##_fw_impl(const Tensor &x, float k, Tensor &y) { \
  float *dest = DATA(y); \
  const float *src = CDATA(x); \
  const unsigned size = x.shape().size(); \
  REPEAT_OP(i, size, dest[i] = (op)); \
}

#define CPUDEV_BW_X_CONST(name, op) \
void Naive::name##_bw_impl( \
    const Tensor &x, const Tensor &y, const Tensor &gy, float k, Tensor &gx) { \
  const float *px = CDATA(x); static_cast<void>(px); \
  const float *py = CDATA(y); static_cast<void>(py); \
  const float *pgy = CDATA(gy); \
  float *pgx = DATA(gx); \
  const unsigned size = x.shape().size(); \
  REPEAT_OP(i, size, pgx[i] += (op)); \
}

#define CPUDEV_FW_X_SCALAR(name, op) \
void Naive::name##_fw_impl(const Tensor &x, const Tensor &k, Tensor &y) { \
  const unsigned size = y.shape().volume(); \
  const unsigned bs = y.shape().batch(); \
  const unsigned skip_x = x.shape().has_batch() * size; \
  const unsigned skip_k = k.shape().has_batch(); \
  float *dest = DATA(y); \
  const float *src_x = CDATA(x); \
  const float *src_k = CDATA(k); \
  for (unsigned batch = 0; batch < bs; ++batch) { \
    REPEAT_OP(i, size, dest[i] = (op)); \
    dest += size; \
    src_x += skip_x; \
    src_k += skip_k; \
  } \
}

#define CPUDEV_FW_AB(name, op) \
void Naive::name##_fw_impl(const Tensor &a, const Tensor &b, Tensor &y) { \
  const unsigned size = y.shape().volume(); \
  const unsigned bs = y.shape().batch(); \
  const unsigned skip_a = a.shape().has_batch() * size; \
  const unsigned skip_b = b.shape().has_batch() * size; \
  float *dest = DATA(y); \
  const float *src_a = CDATA(a); \
  const float *src_b = CDATA(b); \
  for (unsigned batch = 0; batch < bs; ++batch) { \
    REPEAT_OP(i, size, dest[i] = (op)); \
    dest += size; \
    src_a += skip_a; \
    src_b += skip_b; \
  } \
}

CPUDEV_FW_X(negate, -src[i]);
CPUDEV_FW_X(sqrt, std::sqrt(src[i]));
CPUDEV_FW_X(exp, std::exp(src[i]));
CPUDEV_FW_X(log, std::log(src[i]));
CPUDEV_FW_X(tanh, std::tanh(src[i]));
CPUDEV_FW_X(sigmoid, .5 + .5 * std::tanh(.5 * src[i]));
CPUDEV_FW_X(
    softplus, src[i] > 0
      ? src[i] + std::log(1 + std::exp(-src[i]))
      : std::log(1 + std::exp(src[i])));
CPUDEV_FW_X(sin, std::sin(src[i]));
CPUDEV_FW_X(cos, std::cos(src[i]));
CPUDEV_FW_X(tan, std::tan(src[i]));

CPUDEV_BW_X(sqrt, .5 * pgy[i] / py[i]);
CPUDEV_BW_X(exp, py[i] * pgy[i]);
CPUDEV_BW_X(log, pgy[i] / px[i]);
CPUDEV_BW_X(tanh, (1. - py[i] * py[i]) * pgy[i]);
CPUDEV_BW_X(sigmoid, py[i] * (1. - py[i]) * pgy[i]);
CPUDEV_BW_X(softplus, (.5 + .5 * std::tanh(.5 * px[i])) * pgy[i]);
CPUDEV_BW_X(sin, std::cos(px[i]) * pgy[i]);
CPUDEV_BW_X(cos, -std::sin(px[i]) * pgy[i]);
CPUDEV_BW_X(tan, (1 + py[i] * py[i]) * pgy[i]);

CPUDEV_FW_X_CONST(add_const, src[i] + k);
CPUDEV_FW_X_CONST(subtract_const_r, src[i] - k);
CPUDEV_FW_X_CONST(subtract_const_l, k - src[i]);
CPUDEV_FW_X_CONST(multiply_const, src[i] * k);
CPUDEV_FW_X_CONST(divide_const_r, src[i] / k);
CPUDEV_FW_X_CONST(divide_const_l, k / src[i]);
CPUDEV_FW_X_CONST(prelu, src[i] * ((src[i] > 0) + k * (src[i] <= 0)));
CPUDEV_FW_X_CONST(
    elu, src[i] * (src[i] > 0) + k * (std::exp(src[i] * (src[i] <= 0)) - 1));

CPUDEV_BW_X_CONST(add_const, pgy[i]);
CPUDEV_BW_X_CONST(subtract_const_r, pgy[i]);
CPUDEV_BW_X_CONST(subtract_const_l, -pgy[i]);
CPUDEV_BW_X_CONST(multiply_const, k * pgy[i]);
CPUDEV_BW_X_CONST(divide_const_r, pgy[i] / k);
CPUDEV_BW_X_CONST(divide_const_l, -py[i] * pgy[i] / px[i]);
CPUDEV_BW_X_CONST(prelu, pgy[i] * ((px[i] > 0) + k * (px[i] <= 0)));
CPUDEV_BW_X_CONST(elu, pgy[i] * ((px[i] > 0) + (py[i] + k) * (px[i] <= 0)));

CPUDEV_FW_X_SCALAR(add_scalar, src_x[i] + *src_k);
CPUDEV_FW_X_SCALAR(subtract_scalar_r, src_x[i] - *src_k);
CPUDEV_FW_X_SCALAR(subtract_scalar_l, *src_k - src_x[i]);
CPUDEV_FW_X_SCALAR(multiply_scalar, src_x[i] * *src_k);
CPUDEV_FW_X_SCALAR(divide_scalar_r, src_x[i] / *src_k);
CPUDEV_FW_X_SCALAR(divide_scalar_l, *src_k / src_x[i]);

CPUDEV_FW_AB(add, src_a[i] + src_b[i]);
CPUDEV_FW_AB(subtract, src_a[i] - src_b[i]);
CPUDEV_FW_AB(multiply, src_a[i] * src_b[i]);
CPUDEV_FW_AB(divide, src_a[i] / src_b[i]);

#undef CPUDEV_FW_X
#undef CPUDEV_BW_X
#undef CPUDEV_FW_X_CONST
#undef CPUDEV_BW_X_CONST
#undef CPUDEV_FW_X_SCALAR
#undef CPUDEV_FW_AB

void Naive::add_bw_impl(
    const Tensor &, const Tensor &, const Tensor &, const Tensor &gy,
    Tensor &ga, Tensor &gb) {
  const unsigned size = gy.shape().volume();
  const unsigned bs = gy.shape().batch();
  const unsigned skip_a = ga.shape().has_batch() * size;
  const unsigned skip_b = gb.shape().has_batch() * size;
  const float *pgy = CDATA(gy);
  float *pga = DATA(ga);
  float *pgb = DATA(gb);
  for (unsigned batch = 0; batch < bs; ++batch) {
    for (unsigned i = 0; i < size; ++i) {
      const float k = pgy[i];
      pga[i] += k;
      pgb[i] += k;
    }
    pgy += size;
    pga += skip_a;
    pgb += skip_b;
  }
}

void Naive::subtract_bw_impl(
    const Tensor &, const Tensor &, const Tensor &, const Tensor &gy,
    Tensor &ga, Tensor &gb) {
  const unsigned size = gy.shape().volume();
  const unsigned bs = gy.shape().batch();
  const unsigned skip_a = ga.shape().has_batch() * size;
  const unsigned skip_b = gb.shape().has_batch() * size;
  const float *pgy = CDATA(gy);
  float *pga = DATA(ga);
  float *pgb = DATA(gb);
  for (unsigned batch = 0; batch < bs; ++batch) {
    for (unsigned i = 0; i < size; ++i) {
      const float k = pgy[i];
      pga[i] += k;
      pgb[i] -= k;
    }
    pgy += size;
    pga += skip_a;
    pgb += skip_b;
  }
}

void Naive::multiply_bw_impl(
    const Tensor &a, const Tensor &b, const Tensor &, const Tensor &gy,
    Tensor &ga, Tensor &gb) {
  const unsigned size = gy.shape().volume();
  const unsigned bs = gy.shape().batch();
  const unsigned skip_a = ga.shape().has_batch() * size;
  const unsigned skip_b = gb.shape().has_batch() * size;
  const float *pa = CDATA(a);
  const float *pb = CDATA(b);
  const float *pgy = CDATA(gy);
  float *pga = DATA(ga);
  float *pgb = DATA(gb);
  for (unsigned batch = 0; batch < bs; ++batch) {
    for (unsigned i = 0; i < size; ++i) {
      const float k = pgy[i];
      pga[i] += k * pb[i];
      pgb[i] += k * pa[i];
    }
    pa += skip_a;
    pb += skip_b;
    pgy += size;
    pga += skip_a;
    pgb += skip_b;
  }
}

void Naive::divide_bw_impl(
    const Tensor &, const Tensor &b, const Tensor &y, const Tensor &gy,
    Tensor &ga, Tensor &gb) {
  const unsigned size = gy.shape().volume();
  const unsigned bs = gy.shape().batch();
  const unsigned skip_a = ga.shape().has_batch() * size;
  const unsigned skip_b = gb.shape().has_batch() * size;
  const float *pb = CDATA(b);
  const float *py = CDATA(y);
  const float *pgy = CDATA(gy);
  float *pga = DATA(ga);
  float *pgb = DATA(gb);
  for (unsigned batch = 0; batch < bs; ++batch) {
    for (unsigned i = 0; i < size; ++i) {
      const float k = pgy[i] / pb[i];
      pga[i] += k;
      pgb[i] -= k * py[i];
    }
    pb += skip_b;
    py += size;
    pgy += size;
    pga += skip_a;
    pgb += skip_b;
  }
}

void Naive::transpose_fw_impl(const Tensor &x, Tensor &y) {
  const unsigned d1 = x.shape()[0];
  const unsigned d2 = x.shape()[1];
  const unsigned ms = d1 * d2;
  const unsigned bs = y.shape().batch();
  float *dest = DATA(y);
  const float *src = CDATA(x);

  for (unsigned k = 0; k < bs; ++k) {
    float *pd = dest;
    for (unsigned j = 0; j < d2; ++j) {
      float *ppd = pd;
      for (unsigned i = 0; i < d1; ++i) {
        *ppd = *src++;
        ppd += d2;
      }
      ++pd;
    }
    dest += ms;
  }
}

void Naive::matmul_fw_impl(const Tensor &a, const Tensor &b, Tensor &y) {
  const unsigned d1 = a.shape()[0];
  const unsigned d2 = a.shape()[1];
  const unsigned d3 = b.shape()[1];
  const unsigned bs = y.shape().batch();
  const unsigned dest_shift = d1 * d3;
  const unsigned src_a_shift = a.shape().has_batch() * d1 * d2;
  const unsigned src_b_shift = b.shape().has_batch() * d2 * d3;

  float *dest = DATA(y);
  const float *src_a = CDATA(a);
  const float *src_b = CDATA(b);
  for (unsigned batch = 0; batch < bs; ++batch) {
    for (unsigned i = 0; i < d1; ++i) {
      for (unsigned ky = 0, kb = 0; ky < dest_shift; ky += d1, kb += d2) {
        float tmp = 0;
        for (unsigned ja = 0, jb = 0; jb < d2; ja += d1, ++jb) {
          tmp += src_a[i + ja] * src_b[jb + kb];
        }
        dest[i + ky] = tmp;
      }
    }
    dest += dest_shift;
    src_a += src_a_shift;
    src_b += src_b_shift;
  }
}

void Naive::transpose_bw_impl(
    const Tensor &, const Tensor &, const Tensor &gy, Tensor &gx) {
  // TODO(odashi): This code could be slow and requires memory. Fix this.
  inplace_add_impl(transpose_fw(gy), gx);
}

void Naive::matmul_bw_impl(
    const Tensor &a, const Tensor &b, const Tensor &, const Tensor &gy,
    Tensor &ga, Tensor &gb) {
  // TODO(odashi): This code could be slow and requires memory. Fix this.
  inplace_add_impl(matmul_fw(gy, transpose_fw(b)), ga);
  inplace_add_impl(matmul_fw(transpose_fw(a), gy), gb);
}

void Naive::sum_fw_impl(const Tensor &x, unsigned dim, Tensor &y) {
  const unsigned n = x.shape()[dim];
  const unsigned repeat = y.shape().size();
  const unsigned skip1 = y.shape().lower_volume(dim);
  const unsigned skip2 = skip1 * n;
  float *dest = DATA(y);
  const float *src = CDATA(x);
  for (unsigned i = 0; i < repeat; ++i) {
    unsigned offset = i % skip1 + (i / skip1) * skip2;
    float tmp = 0;
    for (unsigned j = 0; j < n; ++j) {
      tmp += src[offset];
      offset += skip1;
    }
    dest[i] = tmp;
  }
}

void Naive::logsumexp_fw_impl(const Tensor &x, unsigned dim, Tensor &y) {
  const unsigned n = x.shape()[dim];
  const unsigned repeat = y.shape().size();
  const unsigned skip1 = y.shape().lower_volume(dim);
  const unsigned skip2 = skip1 * n;
  float *dest = DATA(y);
  const float *src = CDATA(x);
  for (unsigned i = 0; i < repeat; ++i) {
    // TODO(odashi): This calculation might generate large errors.
    unsigned offset = i % skip1 + (i / skip1) * skip2;
    float tmp = src[offset];
    for (unsigned j = 1; j < n; ++j) {
      offset += skip1;
      float arg = src[offset];
      tmp = tmp > arg
        ? tmp + std::log(1. + std::exp(arg - tmp))
        : arg + std::log(1. + std::exp(tmp - arg));
    }
    dest[i] = tmp;
  }
}

void Naive::broadcast_fw_impl(
    const Tensor &x, unsigned dim, unsigned size, Tensor &y) {
  const unsigned repeat = x.shape().size();
  const unsigned skip1 = y.shape().lower_volume(dim);
  const unsigned skip2 = skip1 * size;
  float *dest = DATA(y);
  const float *src = CDATA(x);
  for (unsigned i = 0; i < repeat; ++i) {
    unsigned offset = i % skip1 + (i / skip1) * skip2;
    float tmp = src[i];
    for (unsigned j = 0; j < size; ++j) {
      dest[offset] = tmp;
      offset += skip1;
    }
  }
}

void Naive::batch_sum_fw_impl(const Tensor &x, Tensor &y) {
  float *dest = DATA(y);
  const float *src = CDATA(x);
  const unsigned bs = x.shape().batch();
  const unsigned size = y.shape().size();
  for (unsigned i = 0; i < size; ++i) {
    float temp = 0;
    for (unsigned batch = 0, pos = i; batch < bs; ++batch, pos += size) {
      temp += src[pos];
    }
    dest[i] = temp;
  }
}

void Naive::inplace_multiply_const_impl(float k, Tensor &x) {
  const unsigned size = x.shape().size();
  float *dest = DATA(x);
  REPEAT_OP(i, size, dest[i] *= k);
}

void Naive::inplace_add_impl(const Tensor &x, Tensor &y) {
  const Shape &sx = x.shape();
  const Shape &sy = y.shape();
  const unsigned size = sy.volume();
  const unsigned bs = std::max(sx.batch(), sy.batch());
  const unsigned b_skip_d = sy.has_batch() * size;
  const unsigned b_skip_s = sx.has_batch() * size;
  float *dest = DATA(y);
  const float *src = CDATA(x);
  for (unsigned batch = 0; batch < bs; ++batch) {
    REPEAT_OP(i, size, dest[i] += src[i]);
    dest += b_skip_d;
    src += b_skip_s;
  }
}

void Naive::inplace_subtract_impl(const Tensor &x, Tensor &y) {
  const Shape &sx = x.shape();
  const Shape &sy = y.shape();
  const unsigned size = sy.volume();
  const unsigned bs = std::max(sx.batch(), sy.batch());
  const unsigned b_skip_d = sy.has_batch() * size;
  const unsigned b_skip_s = sx.has_batch() * size;
  float *dest = DATA(y);
  const float *src = CDATA(x);
  for (unsigned batch = 0; batch < bs; ++batch) {
    REPEAT_OP(i, size, dest[i] -= src[i]);
    dest += b_skip_d;
    src += b_skip_s;
  }
}

}  // namespace devices
}  // namespace primitiv
