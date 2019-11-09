#include <assert.h>
#include <lodepng.h>
#include <stdlib.h>

#include "encode.h"
#include "error.h"

typedef struct {
  size_t inputLength;
  unsigned char* input;
  unsigned width, height;

  napi_ref reference;
  napi_deferred deferred;

  size_t outputLength;
  unsigned char* output;
  unsigned error;
} EncodeData;

void encode_execute(napi_env env, void* _data) {
  EncodeData* data = _data;

  if (data->inputLength != data->width * data->height * 4) return;
  data->error = lodepng_encode32(&data->output, &data->outputLength, data->input, data->width, data->height);
}

void encode_free(napi_env env, void* finalize_data, void* finalize_hint) {
  free(finalize_data);
}

void encode_complete(napi_env env, napi_status status, void* _data) {
  EncodeData* data = _data;

  assert(napi_delete_reference(env, data->reference) == napi_ok);

  if (data->inputLength != data->width * data->height * 4) {
    napi_value error;
    assert(create_wrong_input_size_error(env, &error) == napi_ok);
    assert(napi_reject_deferred(env, data->deferred, error) == napi_ok);
    return;
  }

  if (data->error) {
    napi_value error;
    assert(create_lodepng_error(env, data->error, &error) == napi_ok);
    assert(napi_reject_deferred(env, data->deferred, error) == napi_ok);
    return;
  }

  napi_value buffer;
  assert(napi_create_external_buffer(env, data->outputLength, data->output, encode_free, NULL, &buffer) == napi_ok);
  assert(napi_resolve_deferred(env, data->deferred, buffer) == napi_ok);

  free(_data);
}

napi_value encode(napi_env env, napi_callback_info info) {
  size_t argc = 3;
  napi_value args[3];
  assert(napi_get_cb_info(env, info, &argc, args, NULL, NULL) == napi_ok);

  EncodeData* data = malloc(sizeof(EncodeData));
  assert(napi_get_typedarray_info(env, args[0], NULL, &data->inputLength, (void **) &data->input, NULL, NULL) == napi_ok);
  assert(napi_create_reference(env, args[0], 1, &data->reference) == napi_ok);
  assert(napi_get_value_uint32(env, args[1], &data->width) == napi_ok);
  assert(napi_get_value_uint32(env, args[2], &data->height) == napi_ok);

  napi_value promise;
  assert(napi_create_promise(env, &data->deferred, &promise) == napi_ok);

  napi_value work_name;
  assert(napi_create_string_utf8(env, "lodepng:encode", NAPI_AUTO_LENGTH, &work_name) == napi_ok);

  napi_async_work work;
  assert(napi_create_async_work(env, NULL, work_name, encode_execute, encode_complete, (void*) data, &work) == napi_ok);

  assert(napi_queue_async_work(env, work) == napi_ok);

  return promise;
}
