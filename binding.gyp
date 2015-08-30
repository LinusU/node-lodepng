{
  "targets": [{
    "target_name": "lodepng",
    "sources": [ "vendor/lodepng.cpp", "src/decode.cc", "src/encode.cc", "src/module.cc" ],
    "include_dirs" : [
      "<!(node -e \"require('nan')\")",
      "vendor"
    ]
  }]
}
