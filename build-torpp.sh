#!/bin/bash

BUILD_DIR=$1
CMAKE_ARGS="-DCMAKE_BUILD_TYPE=Release -DPYTHON_BINDINGS=OFF -DTORPP=ON -DSO_TEST=OFF"

cmake -S . -B ${BUILD_DIR} ${CMAKE_ARGS}
cmake --build ${BUILD_DIR}
cmake --install ${BUILD_DIR}
