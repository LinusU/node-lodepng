#include <node.h>
#include <nan.h>
#include <lodepng.h>

#include "encode.h"

class EncodeWorker : public Nan::AsyncWorker {
public:
  EncodeWorker (v8::Local<v8::Object> src, Nan::Callback* cb) : Nan::AsyncWorker(cb) {
    width = (unsigned) Nan::Get(src, Nan::New("width").ToLocalChecked()).ToLocalChecked()->Uint32Value();
    height = (unsigned) Nan::Get(src, Nan::New("height").ToLocalChecked()).ToLocalChecked()->Uint32Value();

    v8::Local<v8::Value> data = Nan::Get(src, Nan::New("data").ToLocalChecked()).ToLocalChecked();

    input = (unsigned char*) node::Buffer::Data(data);
    inputLength = node::Buffer::Length(data);
  }

  void Execute () {
    unsigned error;

    if (inputLength != width * height * 4) {
      return SetErrorMessage("Unexpected length of buffer");
    }

    error = lodepng_encode32(&output, &outputLength, input, width, height);
    if (error) return SetErrorMessage(lodepng_error_text(error));
  }

  void HandleOKCallback () {
    v8::Local<v8::Object> data = Nan::NewBuffer((char*) output, outputLength).ToLocalChecked();
    v8::Local<v8::Value> argv[] = { Nan::Null(), data };

    callback->Call(2, argv);
  }

private:
  size_t inputLength;
  unsigned char* input;
  unsigned width, height;

  size_t outputLength;
  unsigned char* output;
};

NAN_METHOD(encode) {
  Nan::Callback *cb = new Nan::Callback(info[1].As<v8::Function>());
  Nan::AsyncQueueWorker(new EncodeWorker(info[0]->ToObject(), cb));
}
