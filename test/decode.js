/* eslint-env mocha */

var fs = require('fs')
var png = require('../')
var path = require('path')
var assert = require('assert')

function getPixel (img, x, y) {
  return img.data.readUInt32LE((y * img.width + x) * 4)
}

describe('Decode', function () {
  it('should decode basn3p01', function (done) {
    var data = fs.readFileSync(path.join(__dirname, 'basn3p01.png'))

    png.decode(data, function (err, img) {
      assert.ifError(err)

      assert.equal(img.width, 32)
      assert.equal(img.height, 32)

      assert.equal(getPixel(img, 18, 10), 0xff22ffee)
      assert.equal(getPixel(img, 14, 26), 0xffff6622)

      done()
    })
  })
})
