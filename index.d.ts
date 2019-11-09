///<reference types="node"/>

import ImageData = require('@canvas/image-data')

interface ImageLike {
  width: number
  height: number
  data: ArrayBuffer | Buffer | Int8Array | Uint8Array | Uint8ClampedArray
}

export function encode (source: ImageLike): Promise<Buffer>
export function decode (source: Buffer): Promise<ImageData>
