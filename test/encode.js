/* eslint-env mocha */

var png = require('../')
var assert = require('assert')
var sizeOf = require('image-size')

describe('Encode', function () {
  it('should encode an image', function (done) {
    var src = new Buffer(8)

    src.writeUInt32LE(0xff22ffee, 0)
    src.writeUInt32LE(0xffff6622, 4)

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
