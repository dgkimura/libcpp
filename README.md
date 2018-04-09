# libcpp

[![Build Status](https://travis-ci.org/dgkimura/libcpp.svg?branch=master)](https://travis-ci.org/dgkimura/libcpp)

The libcpp library provides standard library implementations.


## Build
You will need a compiler with full C++11 support. You can then use CMake to
generate MakeFiles for this project.
```
$ mkdir build && cd build
$ cmake ..
$ make && unittests/all_unittests
```
