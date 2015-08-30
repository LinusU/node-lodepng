#include <node.h>
#include <nan.h>

#include "decode.h"
#include "encode.h"

NAN_MODULE_INIT(Initialize) {
  NAN_EXPORT(target, decode);
  NAN_EXPORT(target, encode);
}

NODE_MODULE(lodepng, Initialize)
