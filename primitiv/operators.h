#ifndef PRIMITIV_OPERATORS_H_
#define PRIMITIV_OPERATORS_H_

#include <initializer_list>
#include <type_traits>
#include <vector>

#include <primitiv/device.h>
#include <primitiv/error.h>
#include <primitiv/graph.h>

namespace primitiv {

namespace type_traits {

// NOTE(odashi):
// The template variable `Var` could becomes only either `Tensor` or `Node`.

template<typename Var> struct is_var : std::false_type {};
template<> struct is_var<Tensor> : std::true_type {};
template<> struct is_var<Node> : std::true_type {};

template<typename Var>
using Identity = typename std::enable_if<is_var<Var>::value, Var>::type;

template<typename Container>
using Reduce = typename std::enable_if<
  is_var<typename Container::value_type>::value,
  typename Container::value_type
>::type;

template<typename Container>
using ReducePtr = typename std::enable_if<
  std::is_pointer<typename Container::value_type>::value &&
  is_var<
    typename std::remove_const<
      typename std::remove_pointer<typename Container::value_type>::type
    >::type
  >::value,
  typename std::remove_const<
    typename std::remove_pointer<typename Container::value_type>::type
  >::type
>::type;

}  // namespace type_traits

class Parameter;

template<typename Var>
type_traits::Identity<Var> operator+(const Var &x);

template<typename Var>
type_traits::Identity<Var> operator-(const Var &x);

template<typename Var>
type_traits::Identity<Var> operator+(const Var &x, float k);

template<typename Var>
type_traits::Identity<Var> operator+(float k, const Var &x);

template<typename Var>
type_traits::Identity<Var> operator+(const Var &a, const Var &b);

template<typename Var>
type_traits::Identity<Var> operator-(const Var &x, float k);

template<typename Var>
type_traits::Identity<Var> operator-(float k, const Var &x);

template<typename Var>
type_traits::Identity<Var> operator-(const Var &a, const Var &b);

template<typename Var>
type_traits::Identity<Var> operator*(const Var &x, float k);

template<typename Var>
type_traits::Identity<Var> operator*(float k, const Var &x);

template<typename Var>
type_traits::Identity<Var> operator*(const Var &a, const Var &b);

template<typename Var>
type_traits::Identity<Var> operator/(const Var &x, float k);

template<typename Var>
type_traits::Identity<Var> operator/(float k, const Var &x);

template<typename Var>
type_traits::Identity<Var> operator/(const Var &a, const Var &b);

namespace operators {

Tensor input_tensor(
    const Shape &shape,
    const std::vector<float> &data,
    Device &dev = Device::get_default_device());

Node input_node(
    const Shape &shape,
    const std::vector<float> &data,
    Device &dev = Device::get_default_device(),
    Graph &g = Graph::get_default_graph());

template<typename Var>
type_traits::Identity<Var> input(
    const Shape &shape,
    const std::vector<float> &data,
    Device &dev = Device::get_default_device());

template<>
inline Tensor input<Tensor>(
    const Shape &shape,
    const std::vector<float> &data,
    Device &dev) {
  return input_tensor(shape, data, dev);
}

template<>
inline Node input<Node>(
    const Shape &shape,
    const std::vector<float> &data,
    Device &dev) {
  return input_node(shape, data, dev);
}

Tensor input_tensor(Parameter &param);

Node input_node(Parameter &param, Graph &g = Graph::get_default_graph());

template<typename Var>
type_traits::Identity<Var> input(Parameter &param);

template<>
inline Tensor input<Tensor>(Parameter &param) {
  return input_tensor(param);
}

template<>
inline Node input<Node>(Parameter &param) {
  return input_node(param);
}

template<typename Var>
type_traits::Identity<Var> copy(
    const Var &x, Device &dev = Device::get_default_device());

template<typename Var>
type_traits::Identity<Var> pick(
    const Var &x, const std::vector<unsigned> &ids, unsigned dim);

template<typename Var>
type_traits::Identity<Var> slice(
    const Var &x, unsigned dim, unsigned lower, unsigned upper);

template<typename Var>
type_traits::Identity<Var> concat(const std::vector<Var> &xs, unsigned dim);

template<typename Var>
type_traits::Identity<Var> concat(
    const std::vector<const Var *> &xs, unsigned dim);

template<typename Var>
type_traits::Identity<Var> concat(
    const std::initializer_list<Var> xs, unsigned dim) {
  return concat(std::vector<Var>(xs), dim);
}

template<typename Var>
type_traits::Identity<Var> concat(
    const std::initializer_list<const Var *> xs, unsigned dim) {
  return concat(std::vector<const Var *>(xs), dim);
}

template<typename Container>
inline type_traits::Reduce<Container> concat(
    const Container &xs, unsigned dim) {
  using Var = type_traits::Reduce<Container>;
  return concat(std::vector<Var>(xs.begin(), xs.end()), dim);
}

template<typename Container>
inline type_traits::ReducePtr<Container> concat(
    const Container &xs, unsigned dim) {
  using Var = type_traits::ReducePtr<Container>;
  return concat(std::vector<const Var *>(xs.begin(), xs.end()), dim);
}

template<typename Var>
type_traits::Identity<Var> reshape(const Var &x, const Shape &new_shape);

template<typename Var>
type_traits::Identity<Var> flatten(const Var &x);

template<typename Var>
type_traits::Identity<Var> transpose(const Var &x);

template<typename Var>
type_traits::Identity<Var> matmul(const Var &a, const Var &b);

template<typename Var>
type_traits::Identity<Var> sqrt(const Var &x);

template<typename Var>
type_traits::Identity<Var> exp(const Var &x);

template<typename Var>
type_traits::Identity<Var> tanh(const Var &x);

template<typename Var>
type_traits::Identity<Var> sigmoid(const Var &x);

template<typename Var>
type_traits::Identity<Var> softplus(const Var &x);

template<typename Var>
type_traits::Identity<Var> sin(const Var &x);

template<typename Var>
type_traits::Identity<Var> cos(const Var &x);

template<typename Var>
type_traits::Identity<Var> tan(const Var &x);

template<typename Var>
type_traits::Identity<Var> relu(const Var &x);

template<typename Var>
type_traits::Identity<Var> lrelu(const Var &x);

template<typename Var>
type_traits::Identity<Var> prelu(const Var &x, float a);

template<typename Var>
type_traits::Identity<Var> elu(const Var &x, float a);

template<typename Var>
inline type_traits::Identity<Var> selu(
    const Var &x,
    float a = 1.6732632423543772848170429916717,
    float s = 1.0507009873554804934193349852946) {
  return s * elu(x, a);
}

template<typename Var>
type_traits::Identity<Var> sum(const Var &x, unsigned dim);

template<typename Container>
inline type_traits::Reduce<Container> sum(const Container &xs) {
  using Var = type_traits::Reduce<Container>;
  if (xs.empty()) THROW_ERROR("No nodes to sum.");
  auto it = xs.begin();
  Var ret = *it++;
  while (it != xs.end()) ret += *it++;
  return ret;
}

template<typename Container>
inline type_traits::ReducePtr<Container> sum(const Container &xs) {
  using Var = type_traits::ReducePtr<Container>;
  if (xs.empty()) THROW_ERROR("No nodes to sum.");
  auto it = xs.begin();
  Var ret = **it++;
  while (it != xs.end()) ret += **it++;
  return ret;
}

template<typename Var>
inline type_traits::Identity<Var> mean(const Var &x, unsigned dim) {
  return sum(x, dim) / x.shape()[dim];
}

template<typename Container>
inline type_traits::Reduce<Container> mean(const Container &xs) {
  return sum(xs) / xs.size();
}

template<typename Container>
inline type_traits::ReducePtr<Container> mean(const Container &xs) {
  return sum(xs) / xs.size();
}

template<typename Var>
type_traits::Identity<Var> broadcast(const Var &x, unsigned dim, unsigned size);

template<typename Var>
type_traits::Identity<Var> logsumexp(const Var &x, unsigned dim);

template<typename Var>
type_traits::Identity<Var> log_softmax(const Var &x, unsigned dim);

template<typename Var>
type_traits::Identity<Var> softmax(const Var &x, unsigned dim);

template<typename Var>
type_traits::Identity<Var> softmax_cross_entropy(
    const Var &x, const Var &t, unsigned dim);

template<typename Var>
type_traits::Identity<Var> softmax_cross_entropy(
    const Var &x, const std::vector<unsigned> &ids, unsigned dim);

namespace batch {

template<typename Var>
type_traits::Identity<Var> sum(const Var &x);

template<typename Var>
inline type_traits::Identity<Var> mean(const Var &x) {
  return sum(x) / x.shape().batch();
}

template<typename Var>
inline type_traits::Identity<Var> normalize(const Var &x) {
  if (!x.shape().has_batch()) return x;  // No meaning of normalization.
  const unsigned b = x.shape().batch();
  const float scale = b / (b - 1.);
  const Var m = mean(x);
  const Var v = scale * (mean(x * x) - m * m);
  return (x - m) / sqrt(v + 1e-8);
}

}  // namespace batch

Tensor constant_tensor(
    const Shape &shape, float k,
    Device &dev = Device::get_default_device());

Node constant_node(
    const Shape &shape, float k,
    Device &dev = Device::get_default_device(),
    Graph &g = Graph::get_default_graph());

template<typename Var>
type_traits::Identity<Var> constant(
    const Shape &shape, float k,
    Device &dev = Device::get_default_device());

template<>
inline Tensor constant<Tensor>(const Shape &shape, float k, Device &dev) {
  return constant_tensor(shape, k, dev);
}

template<>
inline Node constant<Node>(const Shape &shape, float k, Device &dev) {
  return constant_node(shape, k, dev);
}

inline Tensor zeros_tensor(
    const Shape &shape,
    Device &dev = Device::get_default_device()) {
  return constant_tensor(shape, 0, dev);
}

inline Node zeros_node(
    const Shape &shape,
    Device &dev = Device::get_default_device(),
    Graph &g = Graph::get_default_graph()) {
  return constant_node(shape, 0, dev, g);
}

template<typename Var>
inline type_traits::Identity<Var> zeros(
    const Shape &shape,
    Device &dev = Device::get_default_device()) {
  return constant<Var>(shape, 0, dev);
}

inline Tensor ones_tensor(
    const Shape &shape,
    Device &dev = Device::get_default_device()) {
  return constant_tensor(shape, 1, dev);
}

inline Node ones_node(
    const Shape &shape,
    Device &dev = Device::get_default_device(),
    Graph &g = Graph::get_default_graph()) {
  return constant_node(shape, 1, dev, g);
}

template<typename Var>
inline type_traits::Identity<Var> ones(
    const Shape &shape,
    Device &dev = Device::get_default_device()) {
  return constant<Var>(shape, 1, dev);
}

}  // namespace operators

}  // namespace primitiv

#endif  // PRIMITIV_OPERATORS_H_
