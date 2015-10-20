/* eslint-env mocha */

var png = require('../')
var assert = require('assert')
var sizeOf = require('image-size')

function testEncode (src, done) {
  var img = {
    data: src,
    width: 1,
    height: 2
  }

  png.encode(img, function (err, data) {
    assert.ifError(err)

    var size = sizeOf(data)

    assert.equal(size.width, img.width)
    assert.equal(size.height, img.height)

    assert.equal(data.readUInt32LE(0), 0x474e5089)
    assert.equal(data.readUInt32LE(4), 0x0a1a0a0d)

    done()
  })
}

describe('Encode', function () {
  it('should encode from a Buffer', function (done) {
    var src = new Buffer(8)

    src.writeUInt32LE(0xff22ffee, 0)
    src.writeUInt32LE(0xffff6622, 4)

    testEncode(src, done)
  })

  it('should encode from a Uint8ClampedArray', function (done) {
    var src = new Uint8ClampedArray(8)

    src[0] = 0xff
    src[1] = 0x22
    src[2] = 0xff
    src[3] = 0xee
    src[4] = 0xff
    src[5] = 0xff
    src[6] = 0x66
    src[7] = 0x22

    testEncode(src, done)
  })

  it('should encode from a Uint8Array', function (done) {
    var src = new Uint8Array(8)

    src[0] = 0xff
    src[1] = 0x22
    src[2] = 0xff
    src[3] = 0xee
    src[4] = 0xff
    src[5] = 0xff
    src[6] = 0x66
    src[7] = 0x22

    testEncode(src, done)
  })

  it('should encode from a Uint16Array', function (done) {
    var src = new Uint16Array(4)

    src[0] = 0xff22
    src[1] = 0xffee
    src[2] = 0xffff
    src[3] = 0x6622

    testEncode(src, done)
  })

  it('should encode from a Uint32Array', function (done) {
    var src = new Uint32Array(2)

    src[0] = 0xff22ffee
    src[1] = 0xffff6622

    testEncode(src, done)
  })

  it('should give error on incorrect length', function (done) {
    var src = new Buffer(8)

    src.writeUInt32LE(0xff22ffee, 0)
    src.writeUInt32LE(0xffff6622, 4)

    var img = {
      data: src,
      width: 12,
      height: 12
    }

    png.encode(img, function (err) {
      assert.ok(err)

      done()
    })
  })
})
