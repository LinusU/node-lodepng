///<reference types="node"/>

interface ImageLike {
  width: number
  height: number
  data: Buffer | ArrayBufferView
}

interface Image {
  width: number
  height: number
  data: Buffer
}

export function encode (source: ImageLike): Promise<Buffer>
export function decode (source: Buffer): Promise<Image>
