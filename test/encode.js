/* eslint-env mocha */

'use strict'

const png = require('../')

const assert = require('assert')
const assertRejects = require('assert-rejects')
const sizeOf = require('image-size')

function testEncode (src) {
  const img = {
    data: src,
    width: 1,
    height: 2
  }

  return png.encode(img).then((data) => {
    const size = sizeOf(data)

    assert.equal(size.width, img.width)
    assert.equal(size.height, img.height)

    assert.equal(data.readUInt32LE(0), 0x474e5089)
    assert.equal(data.readUInt32LE(4), 0x0a1a0a0d)
  })
}

describe('Encode', function () {
  it('should encode from a Buffer', () => {
    const src = Buffer.allocUnsafe(8)

    src.writeUInt32LE(0xff22ffee, 0)
    src.writeUInt32LE(0xffff6622, 4)

    return testEncode(src)
  })

  it('should encode from a Uint8ClampedArray', () => {
    const src = new Uint8ClampedArray(8)

    src[0] = 0xff
    src[1] = 0x22
    src[2] = 0xff
    src[3] = 0xee
    src[4] = 0xff
    src[5] = 0xff
    src[6] = 0x66
    src[7] = 0x22

    return testEncode(src)
  })

  it('should encode from a Uint8Array', () => {
    const src = new Uint8Array(8)

    src[0] = 0xff
    src[1] = 0x22
    src[2] = 0xff
    src[3] = 0xee
    src[4] = 0xff
    src[5] = 0xff
    src[6] = 0x66
    src[7] = 0x22

    return testEncode(src)
  })

  it('should encode from a Uint16Array', () => {
    const src = new Uint16Array(4)

    src[0] = 0xff22
    src[1] = 0xffee
    src[2] = 0xffff
    src[3] = 0x6622

    return testEncode(src)
  })

  it('should encode from a Uint32Array', () => {
    const src = new Uint32Array(2)

    src[0] = 0xff22ffee
    src[1] = 0xffff6622

    return testEncode(src)
  })

  it('should give error on incorrect length', () => {
    const src = Buffer.allocUnsafe(8)

    src.writeUInt32LE(0xff22ffee, 0)
    src.writeUInt32LE(0xffff6622, 4)

    const img = {
      data: src,
      width: 12,
      height: 12
    }

    return assertRejects(png.encode(img), 'Unexpected length of buffer')
  })
})
