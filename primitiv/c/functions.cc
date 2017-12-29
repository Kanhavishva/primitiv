/* Copyright 2017 The primitiv Authors. All Rights Reserved. */
#include <primitiv/config.h>

#include <vector>

#include <primitiv/functions.h>
#include <primitiv/c/internal.h>
#include <primitiv/c/functions.h>

using primitiv::Node;
using primitiv::Tensor;
using primitiv::c::internal::to_c_ptr;
using primitiv::c::internal::to_cpp_ptr;
using primitiv::c::internal::to_c_ptr_from_value;

#define PRIMITIV_C_IMPL_UNARY_FUNC(name) \
primitiv_Status primitiv_node_func_##name( \
    const primitiv_Node *x, primitiv_Node **y) try { \
  PRIMITIV_C_CHECK_PTR_ARG(x); \
  *y = to_c_ptr_from_value(primitiv::functions::name(*to_cpp_ptr(x))); \
  return ::primitiv_Status::PRIMITIV_OK; \
} PRIMITIV_C_HANDLE_EXCEPTIONS \
primitiv_Status primitiv_tensor_func_##name( \
    const primitiv_Tensor *x, primitiv_Tensor **y) try { \
  PRIMITIV_C_CHECK_PTR_ARG(x); \
  *y = to_c_ptr_from_value(primitiv::functions::name(*to_cpp_ptr(x))); \
  return ::primitiv_Status::PRIMITIV_OK; \
} PRIMITIV_C_HANDLE_EXCEPTIONS \

#define PRIMITIV_C_IMPL_BINARY_OP(name) \
primitiv_Status primitiv_node_func_##name##_node_const( \
    const primitiv_Node *x, float k, primitiv_Node **y) try { \
  PRIMITIV_C_CHECK_PTR_ARG(x); \
  *y = to_c_ptr_from_value(primitiv::functions::name(*to_cpp_ptr(x), k)); \
  return ::primitiv_Status::PRIMITIV_OK; \
} PRIMITIV_C_HANDLE_EXCEPTIONS \
primitiv_Status primitiv_node_func_##name##_const_node( \
    float k, const primitiv_Node *x, primitiv_Node **y) try { \
  PRIMITIV_C_CHECK_PTR_ARG(x); \
  *y = to_c_ptr_from_value(primitiv::functions::name(k, *to_cpp_ptr(x))); \
  return ::primitiv_Status::PRIMITIV_OK; \
} PRIMITIV_C_HANDLE_EXCEPTIONS \
primitiv_Status primitiv_node_func_##name##_node_node( \
    const primitiv_Node *a, const primitiv_Node *b, primitiv_Node **c) try { \
  PRIMITIV_C_CHECK_PTR_ARG(a); \
  PRIMITIV_C_CHECK_PTR_ARG(b); \
  *c = to_c_ptr_from_value( \
      primitiv::functions::name(*to_cpp_ptr(a), *to_cpp_ptr(b))); \
  return ::primitiv_Status::PRIMITIV_OK; \
} PRIMITIV_C_HANDLE_EXCEPTIONS \
primitiv_Status primitiv_tensor_func_##name##_tensor_const( \
    const primitiv_Tensor *x, float k, primitiv_Tensor **y) try { \
  PRIMITIV_C_CHECK_PTR_ARG(x); \
  *y = to_c_ptr_from_value(primitiv::functions::name(*to_cpp_ptr(x), k)); \
  return ::primitiv_Status::PRIMITIV_OK; \
} PRIMITIV_C_HANDLE_EXCEPTIONS \
primitiv_Status primitiv_tensor_func_##name##_const_tensor( \
    float k, const primitiv_Tensor *x, primitiv_Tensor **y) try { \
  PRIMITIV_C_CHECK_PTR_ARG(x); \
  *y = to_c_ptr_from_value(primitiv::functions::name(k, *to_cpp_ptr(x))); \
  return ::primitiv_Status::PRIMITIV_OK; \
} PRIMITIV_C_HANDLE_EXCEPTIONS \
primitiv_Status primitiv_tensor_func_##name##_tensor_tensor( \
    const primitiv_Tensor *a, const primitiv_Tensor *b, \
    primitiv_Tensor **c) try { \
  PRIMITIV_C_CHECK_PTR_ARG(a); \
  PRIMITIV_C_CHECK_PTR_ARG(b); \
  *c = to_c_ptr_from_value( \
      primitiv::functions::name(*to_cpp_ptr(a), *to_cpp_ptr(b))); \
  return ::primitiv_Status::PRIMITIV_OK; \
} PRIMITIV_C_HANDLE_EXCEPTIONS \

extern "C" {

PRIMITIV_C_IMPL_UNARY_FUNC(positive);
PRIMITIV_C_IMPL_UNARY_FUNC(negative);
PRIMITIV_C_IMPL_BINARY_OP(add);
PRIMITIV_C_IMPL_BINARY_OP(subtract);
PRIMITIV_C_IMPL_BINARY_OP(multiply);
PRIMITIV_C_IMPL_BINARY_OP(divide);

primitiv_Status primitiv_node_func_input(
    const primitiv_Shape *shape, const float *data, size_t n,
    primitiv_Device *dev, primitiv_Graph *g, primitiv_Node **node) try {
  PRIMITIV_C_CHECK_PTR_ARG(shape);
  PRIMITIV_C_CHECK_PTR_ARG(data);
  *node = to_c_ptr_from_value(primitiv::functions::input_node(
      *to_cpp_ptr(shape), std::vector<float>(data, data + n), to_cpp_ptr(dev),
      to_cpp_ptr(g)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_input(
    const primitiv_Shape *shape, const float *data, size_t n,
    primitiv_Device *dev, primitiv_Tensor **tensor) try {
  PRIMITIV_C_CHECK_PTR_ARG(shape);
  PRIMITIV_C_CHECK_PTR_ARG(data);
  *tensor = to_c_ptr_from_value(
      primitiv::functions::input_tensor(
        *to_cpp_ptr(shape),
        std::vector<float>(data, data + n),
        to_cpp_ptr(dev)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_parameter(
    primitiv_Parameter *param, primitiv_Graph *g, primitiv_Node **node) try {
  PRIMITIV_C_CHECK_PTR_ARG(param);
  *node = to_c_ptr_from_value(
      primitiv::functions::parameter_node(*to_cpp_ptr(param), to_cpp_ptr(g)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_parameter(
    primitiv_Parameter *param, primitiv_Tensor **tensor) try {
  PRIMITIV_C_CHECK_PTR_ARG(param);
  *tensor = to_c_ptr_from_value(
      primitiv::functions::parameter_tensor(*to_cpp_ptr(param)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_copy(
    const primitiv_Node *x, primitiv_Device *dev, primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(
      primitiv::functions::copy(
        *to_cpp_ptr(x),
        to_cpp_ptr(dev)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_copy(
    const primitiv_Tensor *x, primitiv_Device *dev, primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(
      primitiv::functions::copy(
        *to_cpp_ptr(x),
        to_cpp_ptr(dev)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_pick(
    const primitiv_Node *x, const uint32_t *ids, size_t n, uint32_t dim,
    primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  PRIMITIV_C_CHECK_PTR_ARG(ids);
  *y = to_c_ptr_from_value(primitiv::functions::pick(
     *to_cpp_ptr(x), std::vector<uint32_t>(ids, ids + n), dim));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_pick(
    const primitiv_Tensor *x, const uint32_t *ids, size_t n, uint32_t dim,
    primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  PRIMITIV_C_CHECK_PTR_ARG(ids);
  *y = to_c_ptr_from_value(primitiv::functions::pick(
      *to_cpp_ptr(x), std::vector<uint32_t>(ids, ids + n), dim));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_slice(
    const primitiv_Node *x, uint32_t dim, uint32_t lower, uint32_t upper,
    primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(
      primitiv::functions::slice(*to_cpp_ptr(x), dim, lower, upper));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_slice(
    const primitiv_Tensor *x, uint32_t dim, uint32_t lower, uint32_t upper,
    primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(
      primitiv::functions::slice(*to_cpp_ptr(x), dim, lower, upper));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_concat(
    const primitiv_Node *const *xs, size_t n, uint32_t dim,
    primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(xs);
  const Node *const *_xs = reinterpret_cast<const Node *const *>(xs);
  *y = to_c_ptr_from_value(primitiv::functions::concat(
      std::vector<const Node*>(_xs, _xs + n), dim));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_concat(
    const primitiv_Tensor *const *xs, size_t n, uint32_t dim,
    primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(xs);
  const Tensor *const *_xs = reinterpret_cast<const Tensor *const *>(xs);
  *y = to_c_ptr_from_value(primitiv::functions::concat(
      std::vector<const Tensor*>(_xs, _xs + n), dim));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_reshape(
    const primitiv_Node *x, const primitiv_Shape *new_shape,
    primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  PRIMITIV_C_CHECK_PTR_ARG(new_shape);
  *y = to_c_ptr_from_value(
      primitiv::functions::reshape(*to_cpp_ptr(x), *to_cpp_ptr(new_shape)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_reshape(
    const primitiv_Tensor *x, const primitiv_Shape *new_shape,
    primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  PRIMITIV_C_CHECK_PTR_ARG(new_shape);
  *y = to_c_ptr_from_value(
      primitiv::functions::reshape(*to_cpp_ptr(x), *to_cpp_ptr(new_shape)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

PRIMITIV_C_IMPL_UNARY_FUNC(flatten);
PRIMITIV_C_IMPL_UNARY_FUNC(transpose);

primitiv_Status primitiv_node_func_matmul(
    const primitiv_Node *a, const primitiv_Node *b, primitiv_Node **c) try {
  PRIMITIV_C_CHECK_PTR_ARG(a);
  PRIMITIV_C_CHECK_PTR_ARG(b);
  *c = to_c_ptr_from_value(
      primitiv::functions::matmul(*to_cpp_ptr(a), *to_cpp_ptr(b)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_matmul(
    const primitiv_Tensor *a, const primitiv_Tensor *b,
    primitiv_Tensor **c) try {
  PRIMITIV_C_CHECK_PTR_ARG(a);
  PRIMITIV_C_CHECK_PTR_ARG(b);
  *c = to_c_ptr_from_value(
      primitiv::functions::matmul(*to_cpp_ptr(a), *to_cpp_ptr(b)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

PRIMITIV_C_IMPL_UNARY_FUNC(sqrt);
PRIMITIV_C_IMPL_UNARY_FUNC(exp);
PRIMITIV_C_IMPL_UNARY_FUNC(log);
PRIMITIV_C_IMPL_UNARY_FUNC(tanh);
PRIMITIV_C_IMPL_UNARY_FUNC(sigmoid);
PRIMITIV_C_IMPL_UNARY_FUNC(softplus);
PRIMITIV_C_IMPL_UNARY_FUNC(sin);
PRIMITIV_C_IMPL_UNARY_FUNC(cos);
PRIMITIV_C_IMPL_UNARY_FUNC(tan);
PRIMITIV_C_IMPL_UNARY_FUNC(relu);
PRIMITIV_C_IMPL_UNARY_FUNC(lrelu);

primitiv_Status primitiv_node_func_prelu(
    const primitiv_Node *x, float a, primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(primitiv::functions::prelu(*to_cpp_ptr(x), a));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_prelu(
    const primitiv_Tensor *x, float a, primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(primitiv::functions::prelu(*to_cpp_ptr(x), a));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_elu(
    const primitiv_Node *x, float a, primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(primitiv::functions::elu(*to_cpp_ptr(x), a));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_elu(
    const primitiv_Tensor *x, float a, primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(primitiv::functions::elu(*to_cpp_ptr(x), a));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_sum(
    const primitiv_Node *x, uint32_t dim, primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(primitiv::functions::sum(*to_cpp_ptr(x), dim));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_sum(
    const primitiv_Tensor *x, uint32_t dim, primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(primitiv::functions::sum(*to_cpp_ptr(x), dim));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_broadcast(
    const primitiv_Node *x, uint32_t dim, uint32_t size,
    primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(
      primitiv::functions::broadcast(*to_cpp_ptr(x), dim, size));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_broadcast(
    const primitiv_Tensor *x, uint32_t dim, uint32_t size,
    primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(
      primitiv::functions::broadcast(*to_cpp_ptr(x), dim, size));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_logsumexp(
    const primitiv_Node *x, uint32_t dim, primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(
      primitiv::functions::logsumexp(*to_cpp_ptr(x), dim));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_logsumexp(
    const primitiv_Tensor *x, uint32_t dim, primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(
      primitiv::functions::logsumexp(*to_cpp_ptr(x), dim));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_log_softmax(
    const primitiv_Node *x, uint32_t dim, primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(
      primitiv::functions::log_softmax(*to_cpp_ptr(x), dim));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_log_softmax(
    const primitiv_Tensor *x, uint32_t dim, primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(
      primitiv::functions::log_softmax(*to_cpp_ptr(x), dim));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_softmax(
    const primitiv_Node *x, uint32_t dim, primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(primitiv::functions::softmax(*to_cpp_ptr(x), dim));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_softmax(
    const primitiv_Tensor *x, uint32_t dim, primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(primitiv::functions::softmax(*to_cpp_ptr(x), dim));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_softmax_cross_entropy(
    const primitiv_Node *x, const primitiv_Node *t, uint32_t dim,
    primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  PRIMITIV_C_CHECK_PTR_ARG(t);
  *y = to_c_ptr_from_value(
      primitiv::functions::softmax_cross_entropy(
          *to_cpp_ptr(x), *to_cpp_ptr(t), dim));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_softmax_cross_entropy(
    const primitiv_Tensor *x, const primitiv_Tensor *t, uint32_t dim,
    primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  PRIMITIV_C_CHECK_PTR_ARG(t);
  *y = to_c_ptr_from_value(
      primitiv::functions::softmax_cross_entropy(
          *to_cpp_ptr(x), *to_cpp_ptr(t), dim));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_softmax_cross_entropy_with_array(
    const primitiv_Node *x, const uint32_t *ids, size_t n, uint32_t dim,
    primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  PRIMITIV_C_CHECK_PTR_ARG(ids);
  *y = to_c_ptr_from_value(
      primitiv::functions::softmax_cross_entropy(
          *to_cpp_ptr(x), std::vector<uint32_t>(ids, ids + n), dim));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_softmax_cross_entropy_with_array(
    const primitiv_Tensor *x, const uint32_t *ids, size_t n, uint32_t dim,
    primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  PRIMITIV_C_CHECK_PTR_ARG(ids);
  *y = to_c_ptr_from_value(
      primitiv::functions::softmax_cross_entropy(
          *to_cpp_ptr(x), std::vector<uint32_t>(ids, ids + n), dim));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

PRIMITIV_C_IMPL_UNARY_FUNC(stop_gradient);

primitiv_Status primitiv_node_func_batch_sum(
    const primitiv_Node *x, primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(primitiv::functions::batch::sum(*to_cpp_ptr(x)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_batch_sum(
    const primitiv_Tensor *x, primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(primitiv::functions::batch::sum(*to_cpp_ptr(x)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_constant(
    const primitiv_Shape *shape, float k, primitiv_Device *dev,
    primitiv_Graph *g, primitiv_Node **node) try {
  PRIMITIV_C_CHECK_PTR_ARG(shape);
  *node = to_c_ptr_from_value(primitiv::functions::constant_node(
      *to_cpp_ptr(shape), k, to_cpp_ptr(dev), to_cpp_ptr(g)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_constant(
    const primitiv_Shape *shape, float k, primitiv_Device *dev,
    primitiv_Tensor **tensor) try {
  PRIMITIV_C_CHECK_PTR_ARG(shape);
  *tensor = to_c_ptr_from_value(
      primitiv::functions::constant_tensor(
          *to_cpp_ptr(shape), k, to_cpp_ptr(dev)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_identity(
    uint32_t size, primitiv_Device *dev, primitiv_Graph *g,
    primitiv_Node **node) try {
  *node = to_c_ptr_from_value(
      primitiv::functions::identity_node(
          size, to_cpp_ptr(dev), to_cpp_ptr(g)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_identity(
    uint32_t size, primitiv_Device *dev, primitiv_Tensor **tensor) try {
  *tensor = to_c_ptr_from_value(
      primitiv::functions::identity_tensor(size, to_cpp_ptr(dev)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_random_bernoulli(
    const primitiv_Shape *shape, float p, primitiv_Device *dev,
    primitiv_Graph *g, primitiv_Node **node) try {
  PRIMITIV_C_CHECK_PTR_ARG(shape);
  *node = to_c_ptr_from_value(primitiv::functions::random::bernoulli_node(
      *to_cpp_ptr(shape), p, to_cpp_ptr(dev), to_cpp_ptr(g)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_random_bernoulli(
    const primitiv_Shape *shape, float p, primitiv_Device *dev,
    primitiv_Tensor **tensor) try {
  PRIMITIV_C_CHECK_PTR_ARG(shape);
  *tensor = to_c_ptr_from_value(primitiv::functions::random::bernoulli_tensor(
      *to_cpp_ptr(shape), p, to_cpp_ptr(dev)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_random_uniform(
    const primitiv_Shape *shape, float lower, float upper, primitiv_Device *dev,
    primitiv_Graph *g, primitiv_Node **node) try {
  PRIMITIV_C_CHECK_PTR_ARG(shape);
  *node = to_c_ptr_from_value(primitiv::functions::random::uniform_node(
      *to_cpp_ptr(shape), lower, upper, to_cpp_ptr(dev), to_cpp_ptr(g)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_random_uniform(
    const primitiv_Shape *shape, float lower, float upper, primitiv_Device *dev,
    primitiv_Tensor **tensor) try {
  PRIMITIV_C_CHECK_PTR_ARG(shape);
  *tensor = to_c_ptr_from_value(primitiv::functions::random::uniform_tensor(
      *to_cpp_ptr(shape), lower, upper, to_cpp_ptr(dev)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_random_normal(
    const primitiv_Shape *shape, float mean, float sd, primitiv_Device *dev,
    primitiv_Graph *g, primitiv_Node **node) try {
  PRIMITIV_C_CHECK_PTR_ARG(shape);
  *node = to_c_ptr_from_value(primitiv::functions::random::normal_node(
      *to_cpp_ptr(shape), mean, sd, to_cpp_ptr(dev), to_cpp_ptr(g)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_random_normal(
    const primitiv_Shape *shape, float mean, float sd, primitiv_Device *dev,
    primitiv_Tensor **tensor) try {
  PRIMITIV_C_CHECK_PTR_ARG(shape);
  *tensor = to_c_ptr_from_value(primitiv::functions::random::normal_tensor(
      *to_cpp_ptr(shape), mean, sd, to_cpp_ptr(dev)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_random_log_normal(
    const primitiv_Shape *shape, float mean, float sd, primitiv_Device *dev,
    primitiv_Graph *g, primitiv_Node **node) try {
  PRIMITIV_C_CHECK_PTR_ARG(shape);
  *node = to_c_ptr_from_value(primitiv::functions::random::log_normal_node(
      *to_cpp_ptr(shape), mean, sd, to_cpp_ptr(dev), to_cpp_ptr(g)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_random_log_normal(
    const primitiv_Shape *shape, float mean, float sd, primitiv_Device *dev,
    primitiv_Tensor **tensor) try {
  PRIMITIV_C_CHECK_PTR_ARG(shape);
  *tensor = to_c_ptr_from_value(
      primitiv::functions::random::log_normal_tensor(
          *to_cpp_ptr(shape), mean, sd, to_cpp_ptr(dev)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_random_gumbel(
    const primitiv_Shape *shape, float mu, float beta, primitiv_Device *dev,
    primitiv_Graph *g, primitiv_Node **node) try {
  PRIMITIV_C_CHECK_PTR_ARG(shape);
  *node = to_c_ptr_from_value(primitiv::functions::random::gumbel_node(
      *to_cpp_ptr(shape), mu, beta, to_cpp_ptr(dev), to_cpp_ptr(g)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_random_gumbel(
    const primitiv_Shape *shape, float mu, float beta, primitiv_Device *dev,
    primitiv_Tensor **tensor) try {
  PRIMITIV_C_CHECK_PTR_ARG(shape);
  *tensor = to_c_ptr_from_value(primitiv::functions::random::gumbel_tensor(
      *to_cpp_ptr(shape), mu, beta, to_cpp_ptr(dev)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

PRIMITIV_C_IMPL_BINARY_OP(pow);

primitiv_Status primitiv_node_func_pown(
    const primitiv_Node *x, uint32_t k, primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(primitiv::functions::pown(*to_cpp_ptr(x), k));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_pown(
    const primitiv_Tensor *x, uint32_t k, primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(primitiv::functions::pown(*to_cpp_ptr(x), k));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

PRIMITIV_C_IMPL_UNARY_FUNC(selu);

primitiv_Status primitiv_node_func_sum_nodes(
    const primitiv_Node *const *xs, size_t n, primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(xs);
  const Node *const *_xs = reinterpret_cast<const Node *const *>(xs);
  *y = to_c_ptr_from_value(
      primitiv::functions::sum(std::vector<const Node*>(_xs, _xs + n)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_sum_tensors(
    const primitiv_Tensor *const *xs, size_t n, primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(xs);
  const Tensor *const *_xs = reinterpret_cast<const Tensor *const *>(xs);
  *y = to_c_ptr_from_value(
      primitiv::functions::sum(std::vector<const Tensor*>(_xs, _xs + n)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_mean(
    const primitiv_Node *x, uint32_t dim, primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(primitiv::functions::mean(*to_cpp_ptr(x), dim));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_mean(
    const primitiv_Tensor *x, uint32_t dim, primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(primitiv::functions::mean(*to_cpp_ptr(x), dim));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_mean_nodes(
    const primitiv_Node *const *xs, size_t n, primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(xs);
  const Node *const *_xs = reinterpret_cast<const Node *const *>(xs);
  *y = to_c_ptr_from_value(
      primitiv::functions::mean(std::vector<const Node*>(_xs, _xs + n)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_mean_tensors(
    const primitiv_Tensor *const *xs, size_t n, primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(xs);
  const Tensor *const *_xs = reinterpret_cast<const Tensor *const *>(xs);
  *y = to_c_ptr_from_value(
      primitiv::functions::mean(std::vector<const Tensor*>(_xs, _xs + n)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_batch_mean(
    const primitiv_Node *x, primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(primitiv::functions::batch::mean(*to_cpp_ptr(x)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_batch_mean(
    const primitiv_Tensor *x, primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(primitiv::functions::batch::mean(*to_cpp_ptr(x)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_batch_normalize(
    const primitiv_Node *x, primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(
      primitiv::functions::batch::normalize(*to_cpp_ptr(x)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_batch_normalize(
    const primitiv_Tensor *x, primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(
      primitiv::functions::batch::normalize(*to_cpp_ptr(x)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_zeros(
    const primitiv_Shape *shape, primitiv_Device *dev, primitiv_Graph *g,
    primitiv_Node **node) try {
  PRIMITIV_C_CHECK_PTR_ARG(shape);
  *node = to_c_ptr_from_value(
      primitiv::functions::zeros_node(
          *to_cpp_ptr(shape), to_cpp_ptr(dev), to_cpp_ptr(g)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_zeros(
    const primitiv_Shape *shape, primitiv_Device *dev,
    primitiv_Tensor **tensor) try {
  PRIMITIV_C_CHECK_PTR_ARG(shape);
  *tensor = to_c_ptr_from_value(
      primitiv::functions::zeros_tensor(*to_cpp_ptr(shape), to_cpp_ptr(dev)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_ones(
    const primitiv_Shape *shape, primitiv_Device *dev, primitiv_Graph *g,
    primitiv_Node **node) try {
  PRIMITIV_C_CHECK_PTR_ARG(shape);
  *node = to_c_ptr_from_value(
      primitiv::functions::ones_node(
          *to_cpp_ptr(shape), to_cpp_ptr(dev), to_cpp_ptr(g)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_ones(
    const primitiv_Shape *shape, primitiv_Device *dev,
    primitiv_Tensor **tensor) try {
  PRIMITIV_C_CHECK_PTR_ARG(shape);
  *tensor = to_c_ptr_from_value(
      primitiv::functions::ones_tensor(*to_cpp_ptr(shape), to_cpp_ptr(dev)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_node_func_dropout(
    const primitiv_Node *x, float rate, _Bool enabled, primitiv_Node **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(
      primitiv::functions::dropout(*to_cpp_ptr(x), rate, enabled));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_tensor_func_dropout(
    const primitiv_Tensor *x, float rate, _Bool enabled,
    primitiv_Tensor **y) try {
  PRIMITIV_C_CHECK_PTR_ARG(x);
  *y = to_c_ptr_from_value(
      primitiv::functions::dropout(*to_cpp_ptr(x), rate, enabled));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

}  // end extern "C"

#undef PRIMITIV_C_IMPL_UNARY_FUNC
#undef PRIMITIV_C_IMPL_BINARY_OP
