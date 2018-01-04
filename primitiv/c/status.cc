#include <primitiv/config.h>

#include <string>

#include <primitiv/c/internal.h>
#include <primitiv/c/status.h>

using primitiv::c::internal::ErrorHandler;

PRIMITIV_C_STATUS primitiv_reset() try {
  ErrorHandler::get_instance().reset();
  return PRIMITIV_C_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS

PRIMITIV_C_STATUS primitiv_get_message(
    char *buffer, size_t *buffer_size) try {
  PRIMITIV_C_CHECK_NOT_NULL(buffer_size);
  primitiv::c::internal::copy_string_to_array(
      ErrorHandler::get_instance().get_message(), buffer, buffer_size);
  return PRIMITIV_C_OK;
} PRIMITIV_C_HANDLE_EXCEPTIONS
