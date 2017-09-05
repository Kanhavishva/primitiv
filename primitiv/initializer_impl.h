#ifndef PRIMITIV_INITIALIZER_IMPL_H_
#define PRIMITIV_INITIALIZER_IMPL_H_

#include <primitiv/initializer.h>

namespace primitiv {
namespace initializers {

/**
 * Initializer class to generate a same-value tensor.
 */
class Constant : public Initializer {
  Constant() = delete;
  Constant(const Constant &) = delete;
  Constant(Constant &&) = delete;
  Constant &operator=(const Constant &) = delete;
  Constant &operator=(Constant &&) = delete;

public:
  /**
   * Crates a new initializer object.
   * @param k Constant to provide.
   */
  explicit Constant(const float k) : k_(k) {}

  void apply(Tensor &x) const override;

private:
  float k_;
};

/**
 * Initializing with the parameterized uniform distribution.
 */
class Uniform : public Initializer {
  Uniform(const Uniform &) = delete;
  Uniform(Uniform &&) = delete;
  Uniform &operator=(const Uniform &) = delete;
  Uniform &operator=(Uniform &&) = delete;

public:
  Uniform(const float lower, const float upper) : lower_(lower), upper_(upper) {}

  void apply(Tensor &x) const override;

private:
  float lower_;
  float upper_;
};

/**
 * The Xavier matrix initialization with the uniform distribution.
 */
class XavierUniform : public Initializer {
  XavierUniform(const XavierUniform &) = delete;
  XavierUniform(XavierUniform &&) = delete;
  XavierUniform &operator=(const XavierUniform &) = delete;
  XavierUniform &operator=(XavierUniform &&) = delete;

public:
  XavierUniform() {}

  void apply(Tensor &x) const override;
};

}  // namespace initializers
}  // namespace primitiv

#endif  // PRIMITIV_INITIALIZER_IMPL_H_
