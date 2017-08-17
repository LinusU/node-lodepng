'use strict'

const api = require('./build/Release/lodepng.node')

exports.encode = function (source) {
  return new Promise((resolve, reject) => {
    api.encode(source, (err, result) => (err ? reject(err) : resolve(result)))
  })
}

exports.decode = function (source) {
  return new Promise((resolve, reject) => {
    api.decode(source, (err, result) => (err ? reject(err) : resolve(result)))
  })
}
