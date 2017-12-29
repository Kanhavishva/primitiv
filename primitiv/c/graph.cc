/* Copyright 2017 The primitiv Authors. All Rights Reserved. */
#include <primitiv/config.h>

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include <primitiv/graph.h>
#include <primitiv/c/internal.h>
#include <primitiv/c/graph.h>

using primitiv::Node;
using primitiv::Graph;
using primitiv::c::internal::to_c_ptr;
using primitiv::c::internal::to_cpp_ptr;
using primitiv::c::internal::to_c_ptr_from_value;

extern "C" {

primitiv_Status primitiv_Node_new(primitiv_Node **node) try {
  *node = to_c_ptr(new Node);
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Node_clone(
    primitiv_Node *src, primitiv_Node **node) try {
  PRIMITIV_C_CHECK_PTR_ARG(src);
  *node = to_c_ptr(new Node(*to_cpp_ptr(src)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Node_delete(primitiv_Node *node) try {
  PRIMITIV_C_CHECK_PTR_ARG(node);
  delete to_cpp_ptr(node);
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Node_valid(
    const primitiv_Node *node, unsigned char *valid) try {
  PRIMITIV_C_CHECK_PTR_ARG(node);
  *valid = to_cpp_ptr(node)->valid();
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Node_graph(
    const primitiv_Node *node, primitiv_Graph **graph) try {
  PRIMITIV_C_CHECK_PTR_ARG(node);
  *graph = to_c_ptr(&to_cpp_ptr(node)->graph());
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Node_operator_id(
    const primitiv_Node *node, uint32_t *id) try {
  PRIMITIV_C_CHECK_PTR_ARG(node);
  *id = to_cpp_ptr(node)->operator_id();
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Node_value_id(
    const primitiv_Node *node, uint32_t *id) try {
  PRIMITIV_C_CHECK_PTR_ARG(node);
  *id = to_cpp_ptr(node)->value_id();
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Node_shape(
    const primitiv_Node *node, primitiv_Shape **shape) try {
  PRIMITIV_C_CHECK_PTR_ARG(node);
  *shape = to_c_ptr_from_value(to_cpp_ptr(node)->shape());
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Node_device(
    const primitiv_Node *node, primitiv_Device **device) try {
  PRIMITIV_C_CHECK_PTR_ARG(node);
  *device = to_c_ptr(&to_cpp_ptr(node)->device());
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Node_to_float(
    const primitiv_Node *node, float *value) try {
  PRIMITIV_C_CHECK_PTR_ARG(node);
  *value = to_cpp_ptr(node)->to_float();
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Node_to_array(
    const primitiv_Node *node, float *array, size_t *array_size) try {
  PRIMITIV_C_CHECK_PTR_ARG(node);
  const std::vector<float> v = to_cpp_ptr(node)->to_vector();
  if (array_size) {
    *array_size = v.size();
  }
  if (array) {
    std::copy(v.begin(), v.end(), array);
  }
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Node_argmax(
    const primitiv_Node *node, uint32_t dim, uint32_t *indices,
    size_t *array_size) try {
  PRIMITIV_C_CHECK_PTR_ARG(node);
  const std::vector<uint32_t> v = to_cpp_ptr(node)->argmax(dim);
  if (array_size) {
    *array_size = v.size();
  }
  if (indices) {
    std::copy(v.begin(), v.end(), indices);
  }
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Node_argmin(
    const primitiv_Node *node, uint32_t dim, uint32_t *indices,
    size_t *array_size) try {
  PRIMITIV_C_CHECK_PTR_ARG(node);
  const std::vector<uint32_t> v = to_cpp_ptr(node)->argmin(dim);
  if (array_size) {
    *array_size = v.size();
  }
  if (indices) {
    std::copy(v.begin(), v.end(), indices);
  }
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Node_backward(const primitiv_Node *node) try {
  PRIMITIV_C_CHECK_PTR_ARG(node);
  to_cpp_ptr(node)->backward();
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Graph_new(primitiv_Graph **graph) try {
  *graph = to_c_ptr(new Graph());
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Graph_delete(primitiv_Graph *graph) try {
  PRIMITIV_C_CHECK_PTR_ARG(graph);
  delete to_cpp_ptr(graph);
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Graph_get_default(primitiv_Graph **graph) try {
  *graph = to_c_ptr(&Graph::get_default());
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Graph_set_default(primitiv_Graph *graph) try {
  PRIMITIV_C_CHECK_PTR_ARG(graph);
  Graph::set_default(*to_cpp_ptr(graph));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Graph_clear(primitiv_Graph *graph) try {
  PRIMITIV_C_CHECK_PTR_ARG(graph);
  to_cpp_ptr(graph)->clear();
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Graph_forward(
    primitiv_Graph *graph, const primitiv_Node *node,
    const primitiv_Tensor **tensor) try {
  PRIMITIV_C_CHECK_PTR_ARG(graph);
  PRIMITIV_C_CHECK_PTR_ARG(node);
  *tensor = to_c_ptr(&to_cpp_ptr(graph)->forward(*to_cpp_ptr(node)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Graph_backward(
    primitiv_Graph *graph, const primitiv_Node *node) try {
  PRIMITIV_C_CHECK_PTR_ARG(graph);
  PRIMITIV_C_CHECK_PTR_ARG(node);
  to_cpp_ptr(graph)->backward(*to_cpp_ptr(node));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Graph_get_shape(
    const primitiv_Graph *graph, const primitiv_Node *node,
    primitiv_Shape **shape) try {
  PRIMITIV_C_CHECK_PTR_ARG(graph);
  PRIMITIV_C_CHECK_PTR_ARG(node);
  *shape = to_c_ptr_from_value(
      to_cpp_ptr(graph)->get_shape(*to_cpp_ptr(node)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Graph_get_device(
    const primitiv_Graph *graph, const primitiv_Node *node,
    primitiv_Device **device) try {
  PRIMITIV_C_CHECK_PTR_ARG(graph);
  PRIMITIV_C_CHECK_PTR_ARG(node);
  *device = to_c_ptr(&to_cpp_ptr(graph)->get_device(*to_cpp_ptr(node)));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Graph_dump(
    const primitiv_Graph *graph, const char *format, char *buffer,
    size_t *buffer_size) try {
  PRIMITIV_C_CHECK_PTR_ARG(graph);
  PRIMITIV_C_CHECK_PTR_ARG(format);
  const std::string str = to_cpp_ptr(graph)->dump(format);
  if (buffer_size) {
    *buffer_size = str.length();
  }
  if (buffer) {
    std::strcpy(buffer, str.c_str());
  }
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Graph_num_operators(
    const primitiv_Graph *graph, uint32_t *num) try {
  PRIMITIV_C_CHECK_PTR_ARG(graph);
  *num = to_cpp_ptr(graph)->num_operators();
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

}  // end extern "C"
