# LodePNG Node.js bindings

Bindings for LodePNG which decodes and encodes PNG images.

## Installation

```sh
npm install --save lodepng
```

## Usage

```javascript
var fs = require('fs')
var png = require('lodepng')

/** Decoding **/

var src = fs.readFileSync('test.png')

png.decode(src, function (err, res) {
  if (err) throw err

  console.log('Image size: ' + res.width + 'x' + res.height)
  // res.data is a Buffer with RGBA data
})

/** Encoding **/

var src = fs.readFileSync('test.raw')

png.encode(src, function (err, res) {
  if (err) throw err

  // res is a Buffer with PNG data
})
```

## API

### `png.decode(source, cb)`

Decode PNG data to raw RGBA buffer.

### `png.encode(source, cb)`

Encode raw RGBA buffer to PNG data.
