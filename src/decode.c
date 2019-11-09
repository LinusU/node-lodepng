#include <assert.h>
#include <lodepng.h>
#include <stdlib.h>

#include "decode.h"
#include "error.h"

typedef struct {
  unsigned char* input;
  size_t inputLength;

  napi_deferred deferred;

  unsigned char* output;
  unsigned width, height;
  unsigned error;
} DecodeData;


void decode_execute(napi_env env, void* _data) {
  DecodeData* data = _data;

  data->error = lodepng_decode32(&data->output, &data->width, &data->height, data->input, data->inputLength);
}

void decode_free(napi_env env, void* finalize_data, void* finalize_hint) {
  free(finalize_data);
}

void decode_complete(napi_env env, napi_status status, void* _data) {
  DecodeData* data = _data;

  if (data->error) {
    napi_value error;
    assert(create_lodepng_error(env, data->error, &error) == napi_ok);
    assert(napi_reject_deferred(env, data->deferred, error) == napi_ok);
    return;
  }

  napi_value result;
  assert(napi_create_object(env, &result) == napi_ok);

  napi_value width;
  assert(napi_create_uint32(env, data->width, &width) == napi_ok);
  assert(napi_set_named_property(env, result, "width", width) == napi_ok);

  napi_value height;
  assert(napi_create_uint32(env, data->height, &height) == napi_ok);
  assert(napi_set_named_property(env, result, "height", height) == napi_ok);

  napi_value buffer;
  assert(napi_create_external_arraybuffer(env, data->output, data->width * data->height * 4, decode_free, NULL, &buffer) == napi_ok);
  assert(napi_set_named_property(env, result, "data", buffer) == napi_ok);

  assert(napi_resolve_deferred(env, data->deferred, result) == napi_ok);

  free(_data);
}

napi_value decode(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  assert(napi_get_cb_info(env, info, &argc, args, NULL, NULL) == napi_ok);

  DecodeData* data = malloc(sizeof(DecodeData));

  assert(napi_get_typedarray_info(env, args[0], NULL, &data->inputLength, (void **) &data->input, NULL, NULL) == napi_ok);

  napi_value promise;
  assert(napi_create_promise(env, &data->deferred, &promise) == napi_ok);

  napi_value work_name;
  assert(napi_create_string_utf8(env, "lodepng:decode", NAPI_AUTO_LENGTH, &work_name) == napi_ok);

  napi_async_work work;
  assert(napi_create_async_work(env, NULL, work_name, decode_execute, decode_complete, (void*) data, &work) == napi_ok);

  assert(napi_queue_async_work(env, work) == napi_ok);

  return promise;
}
