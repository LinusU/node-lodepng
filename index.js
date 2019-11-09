const api = require('./build/Release/lodepng.node')
const ImageData = require('@canvas/image-data')

function toByteArray (input) {
  if (input instanceof ArrayBuffer) return new Uint8Array(input)
  if (input instanceof Int8Array) return input
  if (input instanceof Uint8Array) return input
  if (input instanceof Uint8ClampedArray) return input

  throw new TypeError('Expected "source.data" to be an ArrayBuffer or Uint8Array')
}

exports.encode = function encode (source) {
  return api.encode(toByteArray(source.data), source.width, source.height)
}

exports.decode = function decode (source) {
  return api.decode(source).then((result) => new ImageData(new Uint8ClampedArray(result.data), result.width, result.height))
}
