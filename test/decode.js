/* eslint-env mocha */

const fs = require('fs')
const png = require('../')
const path = require('path')
const assert = require('assert')

const ImageData = require('@canvas/image-data')

function getPixel (img, x, y) {
  return Buffer.from(img.data).readUInt32LE((y * img.width + x) * 4)
}

describe('Decode', () => {
  it('should decode basn3p01', () => {
    const data = fs.readFileSync(path.join(__dirname, 'basn3p01.png'))

    return png.decode(data).then((img) => {
      assert.ok(img instanceof ImageData)
      assert.ok(img.data instanceof Uint8ClampedArray)

      assert.strictEqual(img.width, 32)
      assert.strictEqual(img.height, 32)

      assert.strictEqual(getPixel(img, 18, 10), 0xff22ffee)
      assert.strictEqual(getPixel(img, 14, 26), 0xffff6622)
    })
  })
})
