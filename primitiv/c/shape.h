/* Copyright 2017 The primitiv Authors. All Rights Reserved. */

#ifndef PRIMITIV_C_SHAPE_H_
#define PRIMITIV_C_SHAPE_H_

#include <primitiv/c/define.h>
#include <primitiv/c/status.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Opaque type of Shape.
 */
typedef struct primitiv_Shape primitiv_Shape;

/**
 * Creates a new Shape object.
 * @param shape Pointer to receive a handler.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_new(
    primitiv_Shape **shape);

/**
 * Creates a new Shape object.
 * @param dims List of the dimension sizes.
 * @param n Length of the dims.
 * @param batch Batch size.
 * @param shape Pointer to receive a handler.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_new_with_dims(
    const uint32_t *dims, size_t n, uint32_t batch, primitiv_Shape **shape);

/**
 * Deletes the Shape object.
 * @param shape Pointer of a handler.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_delete(
    primitiv_Shape *shape);

/**
 * Returns the size of the i-th dimension.
 * @param shape Pointer of a handler.
 * @param i Dimension number to check.
 * @param dim_size Pointer to receive the size of the i-th dimension.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_op_getitem(
    const primitiv_Shape *shape,
    uint32_t i,
    uint32_t *dim_size);

/**
 * Returns the dimension array.
 * @param shape Pointer of a handler.
 * @param dims Pointer of an array to receive the dimensions.
 * @param n_dims Pointer to receive the number of the dimensions.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_dims(
    const primitiv_Shape *shape, uint32_t *dims, size_t *n_dims);

/**
 * Returns the depth (length of non-1 dimensions) of the shape.
 * @param shape Pointer of a handler.
 * @param depth Pointer to receive the depth of the shape.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_depth(
    const primitiv_Shape *shape, uint32_t *depth);

/**
 * Returns the batch size.
 * @param shape Pointer of a handler.
 * @param batch Pointer to receive the batch size.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_batch(
    const primitiv_Shape *shape, uint32_t *batch);

/**
 * Returns the number of elements in each sample.
 * This value is equal to the product of all dimensions.
 * @param shape Pointer of a handler.
 * @param volume Pointer to receive the number of elements.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_volume(
    const primitiv_Shape *shape, uint32_t *volume);

/**
 * Returns the number of elements in 1 to specified dim.
 * @param shape Pointer of a handler.
 * @param dim Upper bound of the dimension.
 * @param lower_volume Pointer to receive the number of elements that is equal
 *                     to `dims[0] * dims[1] * ... * dims[dim-1]`
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_lower_volume(
    const primitiv_Shape *shape, uint32_t dim, uint32_t *lower_volume);

/**
 * Returns the number of elements in all samples of the mini-batch.
 * This value is equal to `batch() * volume()`.
 * @param shape Pointer of a handler.
 * @param size Pointer to receive the number of elements.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_size(
    const primitiv_Shape *shape, uint32_t *size);

/**
 * Returns a string representation of the shape.
 * The format is: "[n,m,...]xk"
 * @param shape Pointer of a handler.
 * @param buffer Pointer to receive the encoded string.
 * @param buffer_size Pointer to receive a length of the char sequence.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_to_string(
    const primitiv_Shape *shape, char *buffer, size_t *buffer_size);

/**
 * Compares this and other shape.
 * @param shape Pointer of a handler.
 * @param rhs Shape object to compare.
 * @param eq Pointer to receive a result: true if this and rhs are same, false
 *           otherwise.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_op_eq(
    const primitiv_Shape *shape, const primitiv_Shape *rhs, _Bool *eq);

/**
 * Compares this and other shape.
 * @param shape Pointer of a handler.
 * @param rhs Shape object to compare.
 * @param ne Pointer to receive a result: true if this and rhs are not same,
 *           false otherwise.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_op_ne(
    const primitiv_Shape *shape, const primitiv_Shape *rhs, _Bool *ne);

/**
 * Checks whether the shape has minibatch or not.
 * @param shape Pointer of a handler.
 * @param has_batch Poniter to receive a result: true if the shape has
 *                  minibatch, false otherwise.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_has_batch(
    const primitiv_Shape *shape, _Bool *has_batch);

/**
 * Checks whether two batch size is compatible (broadcastable) or not.
 * @param shape Pointer of a handler.
 * @param rhs Shape object to compare.
 * @param has_compatible_batch Pointer to receive a result: true if both batch
 *                             size is compatible, false otherwise.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_has_compatible_batch(
    const primitiv_Shape *shape, const primitiv_Shape *rhs,
    _Bool *has_compatible_batch);

/**
 * Checks whether the shape is a scalar or not.
 * @param shape Pointer of a handler.
 * @param is_scalar Pointer to receive a result: true if the shape is a scalar,
 *                  false otherwise.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_is_scalar(
    const primitiv_Shape *shape, _Bool *is_scalar);

/**
 * Checks whether the shape is a row vector or not.
 * @param shape Pointer of a handler.
 * @param is_row_vector Pointer to receive a result: true if the shape is a row
 *                      vector, false otherwise.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_is_row_vector(
    const primitiv_Shape *shape, _Bool *is_row_vector);

/**
 * Checks whether the shape is a vector or a matrix, or not.
 * @param shape Pointer of a handler.
 * @param is_matrix Pointer to receive a result: true if the shape is a vector
 *                  or a matrix, false otherwise.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_is_matrix(
    const primitiv_Shape *shape, _Bool *is_matrix);

/**
 * Checks whether two shapes have completely same dimensions.
 * @param shape Pointer of a handler.
 * @param rhs Shape object to compare.
 * @param has_same_dims Pointer to receive a result: true if both shape have
 *                      same dimensions, false otherwise.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_has_same_dims(
    const primitiv_Shape *shape, const primitiv_Shape *rhs,
    _Bool *has_same_dims);

/**
 * Checks whether two shapes have same dimensions without an axis.
 * (LOO: leave one out)
 * @param shape Pointer of a handler.
 * @param rhs Shape object to compare.
 * @param dim Dimension to be ignored.
 * @param has_same_loo_dims Pointer to receive a result: true if both shape have
 *                          same dimensions regardless the dimension `dim`,
 *                          false otherwise.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_has_same_loo_dims(
    const primitiv_Shape *shape, const primitiv_Shape *rhs, uint32_t dim,
    _Bool *has_same_loo_dims);

/**
 * Creates a new shape which have one different dimension.
 * @param shape Pointer of a handler.
 * @param dim Dimension to be changed.
 * @param m New size of the dimension `dim`.
 * @param new_shape Pointer for a new shape.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_resize_dim(
    const primitiv_Shape *shape, uint32_t dim, uint32_t m,
    primitiv_Shape **new_shape);

/**
 * Creates a new shape which have specified batch size.
 * @param shape Pointer of a handler.
 * @param batch New batch size.
 * @param new_shape Pointer for a new shape.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_resize_batch(
    const primitiv_Shape *shape, uint32_t batch, primitiv_Shape **new_shape);

/**
 * Directly updates a specified dimension.
 * @param shape Pointer of a handler.
 * @param dim Dimension to be updated.
 * @param m New size of the dimension `dim`.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_update_dim(
    primitiv_Shape *shape, uint32_t dim, uint32_t m);

/**
 * Directly updates the batch size.
 * @param shape Pointer of a handler.
 * @param batch New batch size.
 * @return Status code.
 */
extern PRIMITIV_C_API primitiv_Status primitiv_Shape_update_batch(
    primitiv_Shape *shape, uint32_t batch);

#ifdef __cplusplus
}  // end extern "C"
#endif

#endif  // PRIMITIV_C_SHAPE_H_
