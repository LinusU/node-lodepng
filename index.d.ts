///<reference types="node"/>

interface Source {
  width: number
  height: number
  data: Buffer | ArrayBufferView
}

export function encode (source: Source): Promise<Buffer>
export function decode (source: Buffer): Promise<Source>
