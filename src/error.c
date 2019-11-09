#include <lodepng.h>

#include "error.h"

napi_status create_wrong_input_size_error(napi_env env, napi_value* result) {
  napi_status status;

  napi_value code;
  status = napi_create_string_utf8(env, "EINPUT", NAPI_AUTO_LENGTH, &code);
  if (status != napi_ok) return status;

  napi_value msg;
  status = napi_create_string_utf8(env, "Unexpected length of buffer", NAPI_AUTO_LENGTH, &msg);
  if (status != napi_ok) return status;

  return napi_create_error(env, code, msg, result);
}

napi_status create_lodepng_error(napi_env env, unsigned error, napi_value* result) {
  napi_status status;

  napi_value code;
  status = napi_create_string_utf8(env, "ELODEPNG", NAPI_AUTO_LENGTH, &code);
  if (status != napi_ok) return status;

  napi_value msg;
  status = napi_create_string_utf8(env, lodepng_error_text(error), NAPI_AUTO_LENGTH, &msg);
  if (status != napi_ok) return status;

  return napi_create_error(env, code, msg, result);
}
