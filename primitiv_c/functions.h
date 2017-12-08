#ifndef PRIMITIV_C_FUNCTIONS_H_
#define PRIMITIV_C_FUNCTIONS_H_

#include "primitiv_c/define.h"
#include "primitiv_c/device.h"
#include "primitiv_c/graph.h"
#include "primitiv_c/parameter.h"
#include "primitiv_c/shape.h"
#include "primitiv_c/status.h"
#include "primitiv_c/tensor.h"

#ifdef __cplusplus
extern "C" {
#endif

CAPI extern primitiv_Node *primitiv_node_func_positive(const primitiv_Node *x);
CAPI extern primitiv_Node *safe_primitiv_node_func_positive(
    const primitiv_Node *x, primitiv_Status *status);
CAPI extern primitiv_Tensor *primitiv_tensor_func_positive(
    const primitiv_Tensor *x);
CAPI extern primitiv_Tensor *safe_primitiv_tensor_func_positive(
    const primitiv_Tensor *x, primitiv_Status *status);

CAPI extern primitiv_Node *primitiv_node_func_negative(const primitiv_Node *x);
CAPI extern primitiv_Node *safe_primitiv_node_func_negative(
    const primitiv_Node *x, primitiv_Status *status);
CAPI extern primitiv_Tensor *primitiv_tensor_func_negative(
    const primitiv_Tensor *x);
CAPI extern primitiv_Tensor *safe_primitiv_tensor_func_negative(
    const primitiv_Tensor *x, primitiv_Status *status);

CAPI extern primitiv_Node *primitiv_node_func_add_node_const(
    const primitiv_Node *x, float k);
CAPI extern primitiv_Node *safe_primitiv_node_func_add_node_const(
    const primitiv_Node *x, float k, primitiv_Status *status);
CAPI extern primitiv_Node *primitiv_node_func_add_const_node(
    float k, const primitiv_Node *x);
CAPI extern primitiv_Node *safe_primitiv_node_func_add_const_node(
    float k, const primitiv_Node *x, primitiv_Status *status);
CAPI extern primitiv_Node *primitiv_node_func_add_node_node(
    const primitiv_Node *a, const primitiv_Node *b);
CAPI extern primitiv_Node *safe_primitiv_node_func_add_node_node(
    const primitiv_Node *a, const primitiv_Node *b, primitiv_Status *status);
CAPI extern primitiv_Tensor *primitiv_tensor_func_add_tensor_const(
    const primitiv_Tensor *x, float k);
CAPI extern primitiv_Tensor *safe_primitiv_tensor_func_add_tensor_const(
    const primitiv_Tensor *x, float k, primitiv_Status *status);
CAPI extern primitiv_Tensor *primitiv_tensor_func_add_const_tensor(
    float k, const primitiv_Tensor *x);
CAPI extern primitiv_Tensor *safe_primitiv_tensor_func_add_const_tensor(
    float k, const primitiv_Tensor *x, primitiv_Status *status);
CAPI extern primitiv_Tensor *primitiv_tensor_func_add_tensor_tensor(
    const primitiv_Tensor *a, const primitiv_Tensor *b);
CAPI extern primitiv_Tensor *safe_primitiv_tensor_func_add_tensor_tensor(
    const primitiv_Tensor *a,
    const primitiv_Tensor *b,
    primitiv_Status *status);

CAPI extern primitiv_Node *primitiv_node_func_subtract_node_const(
    const primitiv_Node *x, float k);
CAPI extern primitiv_Node *safe_primitiv_node_func_subtract_node_const(
    const primitiv_Node *x, float k, primitiv_Status *status);
CAPI extern primitiv_Node *primitiv_node_func_subtract_const_node(
    float k, const primitiv_Node *x);
CAPI extern primitiv_Node *safe_primitiv_node_func_subtract_const_node(
    float k, const primitiv_Node *x, primitiv_Status *status);
CAPI extern primitiv_Node *primitiv_node_func_subtract_node_node(
    const primitiv_Node *a, const primitiv_Node *b);
CAPI extern primitiv_Node *safe_primitiv_node_func_subtract_node_node(
    const primitiv_Node *a, const primitiv_Node *b, primitiv_Status *status);
CAPI extern primitiv_Tensor *primitiv_tensor_func_subtract_tensor_const(
    const primitiv_Tensor *x, float k);
CAPI extern primitiv_Tensor *safe_primitiv_tensor_func_subtract_tensor_const(
    const primitiv_Tensor *x, float k, primitiv_Status *status);
CAPI extern primitiv_Tensor *primitiv_tensor_func_subtract_const_tensor(
    float k, const primitiv_Tensor *x);
CAPI extern primitiv_Tensor *safe_primitiv_tensor_func_subtract_const_tensor(
    float k, const primitiv_Tensor *x, primitiv_Status *status);
CAPI extern primitiv_Tensor *primitiv_tensor_func_subtract_tensor_tensor(
    const primitiv_Tensor *a, const primitiv_Tensor *b);
CAPI extern primitiv_Tensor *safe_primitiv_tensor_func_subtract_tensor_tensor(
    const primitiv_Tensor *a,
    const primitiv_Tensor *b,
    primitiv_Status *status);

CAPI extern primitiv_Node *primitiv_node_func_multiply_node_const(
    const primitiv_Node *x, float k);
CAPI extern primitiv_Node *safe_primitiv_node_func_multiply_node_const(
    const primitiv_Node *x, float k, primitiv_Status *status);
CAPI extern primitiv_Node *primitiv_node_func_multiply_const_node(
    float k, const primitiv_Node *x);
CAPI extern primitiv_Node *safe_primitiv_node_func_multiply_const_node(
    float k, const primitiv_Node *x, primitiv_Status *status);
CAPI extern primitiv_Node *primitiv_node_func_multiply_node_node(
    const primitiv_Node *a, const primitiv_Node *b);
CAPI extern primitiv_Node *safe_primitiv_node_func_multiply_node_node(
    const primitiv_Node *a, const primitiv_Node *b, primitiv_Status *status);
CAPI extern primitiv_Tensor *primitiv_tensor_func_multiply_tensor_const(
    const primitiv_Tensor *x, float k);
CAPI extern primitiv_Tensor *safe_primitiv_tensor_func_multiply_tensor_const(
    const primitiv_Tensor *x, float k, primitiv_Status *status);
CAPI extern primitiv_Tensor *primitiv_tensor_func_multiply_const_tensor(
    float k, const primitiv_Tensor *x);
CAPI extern primitiv_Tensor *safe_primitiv_tensor_func_multiply_const_tensor(
    float k, const primitiv_Tensor *x, primitiv_Status *status);
CAPI extern primitiv_Tensor *primitiv_tensor_func_multiply_tensor_tensor(
    const primitiv_Tensor *a, const primitiv_Tensor *b);
CAPI extern primitiv_Tensor *safe_primitiv_tensor_func_multiply_tensor_tensor(
    const primitiv_Tensor *a,
    const primitiv_Tensor *b,
    primitiv_Status *status);

CAPI extern primitiv_Node *primitiv_node_func_divide_node_const(
    const primitiv_Node *x, float k);
CAPI extern primitiv_Node *safe_primitiv_node_func_divide_node_const(
    const primitiv_Node *x, float k, primitiv_Status *status);
CAPI extern primitiv_Node *primitiv_node_func_divide_const_node(
    float k, const primitiv_Node *x);
CAPI extern primitiv_Node *safe_primitiv_node_func_divide_const_node(
    float k, const primitiv_Node *x, primitiv_Status *status);
CAPI extern primitiv_Node *primitiv_node_func_divide_node_node(
    const primitiv_Node *a, const primitiv_Node *b);
CAPI extern primitiv_Node *safe_primitiv_node_func_divide_node_node(
    const primitiv_Node *a, const primitiv_Node *b, primitiv_Status *status);
CAPI extern primitiv_Tensor *primitiv_tensor_func_divide_tensor_const(
    const primitiv_Tensor *x, float k);
CAPI extern primitiv_Tensor *safe_primitiv_tensor_func_divide_tensor_const(
    const primitiv_Tensor *x, float k, primitiv_Status *status);
CAPI extern primitiv_Tensor *primitiv_tensor_func_divide_const_tensor(
    float k, const primitiv_Tensor *x);
CAPI extern primitiv_Tensor *safe_primitiv_tensor_func_divide_const_tensor(
    float k, const primitiv_Tensor *x, primitiv_Status *status);
CAPI extern primitiv_Tensor *primitiv_tensor_func_divide_tensor_tensor(
    const primitiv_Tensor *a, const primitiv_Tensor *b);
CAPI extern primitiv_Tensor *safe_primitiv_tensor_func_divide_tensor_tensor(
    const primitiv_Tensor *a,
    const primitiv_Tensor *b,
    primitiv_Status *status);

CAPI extern primitiv_Node *primitiv_node_func_mean(const primitiv_Node *x,
                                                   uint32_t dim);

CAPI extern primitiv_Tensor *primitiv_tensor_func_mean(const primitiv_Tensor *x,
                                                       uint32_t dim);

CAPI extern primitiv_Node *primitiv_node_func_input(
    const primitiv_Shape *shape,
    const float *data,
    size_t n,
    primitiv_Device *dev,
    primitiv_Graph *g);
CAPI extern primitiv_Node *safe_primitiv_node_func_input(
    const primitiv_Shape *shape,
    const float *data,
    size_t n,
    primitiv_Device *dev,
    primitiv_Graph *g,
    primitiv_Status *status);
CAPI extern primitiv_Tensor *primitiv_tensor_func_input(
    const primitiv_Shape *shape,
    const float *data,
    size_t n,
    primitiv_Device *dev);
CAPI extern primitiv_Tensor *safe_primitiv_tensor_func_input(
    const primitiv_Shape *shape,
    const float *data,
    size_t n,
    primitiv_Device *dev,
    primitiv_Status *status);

CAPI extern primitiv_Node *primitiv_node_func_parameter(
    primitiv_Parameter *param, primitiv_Graph *g);
CAPI extern primitiv_Node *safe_primitiv_node_func_parameter(
    primitiv_Parameter *param, primitiv_Graph *g, primitiv_Status *status);
CAPI extern primitiv_Tensor *primitiv_tensor_func_parameter(
    primitiv_Parameter *param);
CAPI extern primitiv_Tensor *safe_primitiv_tensor_func_parameter(
    primitiv_Parameter *param, primitiv_Status *status);

CAPI extern primitiv_Node *primitiv_node_func_copy(const primitiv_Node *x,
                                                   primitiv_Device *dev);
CAPI extern primitiv_Node *safe_primitiv_node_func_copy(
    const primitiv_Node *x, primitiv_Device *dev, primitiv_Status *status);
CAPI extern primitiv_Tensor *primitiv_tensor_func_copy(const primitiv_Tensor *x,
                                                       primitiv_Device *dev);
CAPI extern primitiv_Tensor *safe_primitiv_tensor_func_copy(
    const primitiv_Tensor *x, primitiv_Device *dev, primitiv_Status *status);

CAPI extern primitiv_Node *primitiv_node_func_pick(const primitiv_Node *x,
                                                   const uint32_t *ids,
                                                   size_t n,
                                                   uint32_t dim);
CAPI extern primitiv_Node *safe_primitiv_node_func_pick(
    const primitiv_Node *x,
    const uint32_t *ids,
    size_t n,
    uint32_t dim,
    primitiv_Status *status);
CAPI extern primitiv_Tensor *primitiv_tensor_func_pick(const primitiv_Tensor *x,
                                                       const uint32_t *ids,
                                                       size_t n,
                                                       uint32_t dim);
CAPI extern primitiv_Tensor *safe_primitiv_tensor_func_pick(
    const primitiv_Tensor *x,
    const uint32_t *ids,
    size_t n,
    uint32_t dim,
    primitiv_Status *status);

CAPI extern primitiv_Node *primitiv_node_func_slice(const primitiv_Node *x,
                                                    uint32_t dim,
                                                    uint32_t lower,
                                                    uint32_t upper);
CAPI extern primitiv_Node *safe_primitiv_node_func_slice(
    const primitiv_Node *x,
    uint32_t dim,
    uint32_t lower,
    uint32_t upper,
    primitiv_Status *status);
CAPI extern primitiv_Tensor *primitiv_tensor_func_slice(
    const primitiv_Tensor *x,
    uint32_t dim,
    uint32_t lower,
    uint32_t upper);
CAPI extern primitiv_Tensor *safe_primitiv_tensor_func_slice(
    const primitiv_Tensor *x,
    uint32_t dim,
    uint32_t lower,
    uint32_t upper,
    primitiv_Status *status);

CAPI extern primitiv_Node *primitiv_node_func_tanh(const primitiv_Node *x);

CAPI extern primitiv_Tensor *primitiv_tensor_func_tanh(
    const primitiv_Tensor *x);

CAPI extern primitiv_Node *primitiv_node_func_matmul(const primitiv_Node *a,
                                                     const primitiv_Node *b);

CAPI extern primitiv_Tensor *primitiv_tensor_func_matmul(
    const primitiv_Tensor *a, const primitiv_Tensor *b);

CAPI extern primitiv_Node *primitiv_node_func_batch_mean(
    const primitiv_Node *x);

CAPI extern primitiv_Tensor *primitiv_tensor_func_batch_mean(
    const primitiv_Tensor *x);

#ifdef __cplusplus
}  // end extern "C"
#endif

#endif  // PRIMITIV_C_FUNCTIONS_H_
