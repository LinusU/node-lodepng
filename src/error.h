#ifndef LD_ERROR_H
#define LD_ERROR_H

#define NAPI_VERSION 1
#include <node_api.h>

napi_status create_wrong_input_size_error(napi_env env, napi_value* result);
napi_status create_lodepng_error(napi_env env, unsigned error, napi_value* result);

#endif
