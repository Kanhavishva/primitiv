/* Copyright 2017 The primitiv Authors. All Rights Reserved. */

#ifndef PRIMITIV_C_TENSOR_H_
#define PRIMITIV_C_TENSOR_H_

#include <primitiv/c/define.h>
#include <primitiv/c/shape.h>
#include <primitiv/c/status.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Opaque type of Tensor.
 */
typedef struct primitiv_Tensor primitiv_Tensor;

/**
 * Creates an invalid Tensor object.
 * @return Pointer of a handler.
 */
CAPI extern primitiv_Tensor *primitiv_Tensor_new();

/**
 * Creates a new Tensor object.
 * @param src Pointer to a source Tensor.
 * @return Pointer of a handler.
 */
CAPI extern primitiv_Tensor *primitiv_Tensor_new_from_tensor(
    primitiv_Tensor *src);

/**
 * Deletes the Tensor object.
 * @param tensor Pointer of a handler.
 */
CAPI extern void primitiv_Tensor_delete(primitiv_Tensor *tensor);

/**
 * Check whether the object is valid or not.
 * @param tensor Pointer of a handler.
 * @return true if the object is valid, false otherwise.
 * @remarks This returns false when the object is created through the default
 *          constructor or the object had been moved.
 */
CAPI extern bool primitiv_Tensor_valid(const primitiv_Tensor *tensor);

/**
 * Returns the shape of the Tensor.
 * @param tensor Pointer of a handler.
 * @param shape Pointer to receive a Shape of the Tensor.
 * @return Status code.
 */
CAPI extern primitiv_Status primitiv_Tensor_shape(const primitiv_Tensor *tensor,
                                                  primitiv_Shape **shape);

/**
 * Returns the Device object related to the internal memory.
 * @param tensor Pointer of a handler.
 * @param device Pointer to receive the Device object.
 * @return Status code.
 */
CAPI extern primitiv_Status primitiv_Tensor_device(
    const primitiv_Tensor *tensor, primitiv_Device **device);

/**
 * Retrieves one internal value in the tensor.
 * @param tensor Pointer of a handler.
 * @param value Pointer to receive an internal float value.
 * @return Status code.
 * @remarks This function can be used only when the tensor is a scalar and
 *          non-minibatched (i.e., shape() == Shape()).
 */
CAPI extern primitiv_Status primitiv_Tensor_to_float(
    const primitiv_Tensor *tensor, float *value);

/**
 * Retrieves internal values in the tensor as a vector.
 * @param tensor Pointer of a handler.
 * @param array Pointer to receive a list of the internal values.
 * @return Status code.
 * @remarks Each resulting values a re ordered by the column-major order, and
 *          the batch size is assumed as the last dimension of the tensor.
 */
CAPI extern primitiv_Status primitiv_Tensor_to_array(
    const primitiv_Tensor *tensor, float *array);

/**
 * Retrieves argmax indices along an axis.
 * @param tensor Pointer of a handler.
 * @param dim A specified axis.
 * @param indices Pointer to receive a list of integers that indicates positions
 *                of the maximum values.
 * @return Status code.
 */
CAPI extern primitiv_Status primitiv_Tensor_argmax(
    const primitiv_Tensor *tensor, uint32_t dim, uint32_t *indices);

/**
 * Retrieves argmin indices along an axis.
 * @param tensor Pointer of a handler.
 * @param dim A specified axis.
 * @param indices Pointer to receive a list of integers that indicates positions
 *                of the minimum values.
 * @return Status code.
 */
CAPI extern primitiv_Status primitiv_Tensor_argmin(
    const primitiv_Tensor *tensor, uint32_t dim, uint32_t *indices);

/**
 * Reset internal values using a constant.
 * @param tensor Pointer of a handler.
 * @return Status code.
 * @param k A value to be used to initialize each element.
 */
CAPI extern primitiv_Status primitiv_Tensor_reset(primitiv_Tensor *tensor,
                                                  float k);

/**
 * Reset internal values using a vector.
 * @param tensor Pointer of a handler.
 * @param values Array of values to be used to initialize each element.
 * @return Status code.
 * @remarks Length of `values` should be equal to `shape().size()`. Each
 *          element should be ordered by the column-major order, and the batch
 *          size is assumed as the last dimension.
 */
CAPI extern primitiv_Status primitiv_Tensor_reset_by_array(
    primitiv_Tensor *tensor, const float *values);

/**
 * Returns a tensor which have the same values and different shape.
 * @param tensor Pointer of a handler.
 * @param new_shape New shape with batch size 1.
 * @param new_tensor Pointer to receive a new tensor.
 * @return Status code.
 */
CAPI extern primitiv_Status primitiv_Tensor_reshape(
    const primitiv_Tensor *tensor, const primitiv_Shape *new_shape,
    primitiv_Tensor **new_tensor);

/**
 * Returns a flattened tensor.
 * @param tensor Pointer of a handler.
 * @param new_tensor Pointer to receive a new tensor.
 * @return Status code.
 */
CAPI extern primitiv_Status primitiv_Tensor_flatten(
    const primitiv_Tensor *tensor, primitiv_Tensor **new_tensor);

/**
 * Directly multiplies a constant.
 * @param tensor Pointer of a handler.
 * @param k A constant to multiply.
 * @return Status code.
 */
CAPI extern primitiv_Status primitiv_Tensor_inplace_multiply_const(
    primitiv_Tensor *tensor, float k);

/**
 * Directly adds a value.
 * @param tensor Pointer of a handler.
 * @param x A tensor to add.
 * @return Status code.
 */
CAPI extern primitiv_Status primitiv_Tensor_inplace_add(
    primitiv_Tensor *tensor, const primitiv_Tensor *x);

/**
 * Directly subtracts a value.
 * @param tensor Pointer of a handler.
 * @param x A tensor to subtract.
 * @return Status code.
 */
CAPI extern primitiv_Status primitiv_Tensor_inplace_subtract(
    primitiv_Tensor *tensor, const primitiv_Tensor *x);

#ifdef __cplusplus
}  // end extern "C"
#endif

#endif  // PRIMITIV_C_TENSOR_H_
