#include <node.h>
#include <nan.h>
#include <lodepng.h>

#include "decode.h"

class DecodeWorker : public Nan::AsyncWorker {
public:
  DecodeWorker (v8::Local<v8::Value> data, Nan::Callback* cb) : Nan::AsyncWorker(cb) {
    raw = (unsigned char*) node::Buffer::Data(data);
    len = node::Buffer::Length(data);
  }

  void Execute () {
    unsigned error;

    error = lodepng_decode32(&output, &width, &height, raw, len);
    if (error) return SetErrorMessage(lodepng_error_text(error));
  }

  void HandleOKCallback () {
    uint32_t size = width * height * 4;
    v8::Local<v8::Object> result = Nan::New<v8::Object>();
    v8::Local<v8::Object> data = Nan::NewBuffer((char*) output, size).ToLocalChecked();

    Nan::Set(result, Nan::New("width").ToLocalChecked(), Nan::New<v8::Integer>(width));
    Nan::Set(result, Nan::New("height").ToLocalChecked(), Nan::New<v8::Integer>(height));
    Nan::Set(result, Nan::New("data").ToLocalChecked(), data);

    v8::Local<v8::Value> argv[] = { Nan::Null(), result };

    callback->Call(2, argv);
  }

private:
  unsigned char* raw;
  size_t len;

  unsigned char* output;
  unsigned width, height;
};

NAN_METHOD(decode) {
  Nan::Callback *cb = new Nan::Callback(info[1].As<v8::Function>());
  Nan::AsyncQueueWorker(new DecodeWorker(info[0], cb));
}
