/* Copyright 2017 The primitiv Authors. All Rights Reserved. */
#include <primitiv/config.h>

#include <primitiv/initializer.h>
#include <primitiv/c/internal.h>
#include <primitiv/c/initializer.h>

using primitiv::Initializer;
using primitiv::c::internal::to_cpp_ptr;

primitiv_Status primitiv_Initializer_delete(
    primitiv_Initializer *initializer) try {
  PRIMITIV_C_CHECK_NOT_NULL(initializer);
  delete to_cpp_ptr(initializer);
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

primitiv_Status primitiv_Initializer_apply(
    const primitiv_Initializer *initializer, primitiv_Tensor *x) try {
  PRIMITIV_C_CHECK_NOT_NULL(initializer);
  to_cpp_ptr(initializer)->apply(*to_cpp_ptr(x));
  return ::primitiv_Status::PRIMITIV_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS
