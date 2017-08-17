# LodePNG Node.js bindings

Bindings for LodePNG which decodes and encodes PNG images.

## Installation

```sh
npm install --save lodepng
```

## Usage

```javascript
const fs = require('fs')
const png = require('lodepng')

/** Decoding **/

const src = fs.readFileSync('test.png')

png.decode(src).then((res) => {
  console.log(`Image size: ${res.width}x${res.height}`)
  // res.data is a Buffer with RGBA data
})

/** Encoding **/

const data = fs.readFileSync('test.raw')

png.encode({ width: 16, height: 16, data }).then((res) => {
  // res is a Buffer with PNG data
})
```

## API

### `png.decode(source) => Promise`

Decode PNG data to raw RGBA buffer.

### `png.encode(source) => Promise`

Encode raw RGBA buffer to PNG data.
