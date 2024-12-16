#!/bin/bash

set -e

if [ -z "$1" ]; then
  echo "Usage: $0 {BUILD_DIR}"
  exit 1
fi

BUILD_DIR=$1
STUBS_DIR="$BUILD_DIR/stubs"
PROJECT=onionpy
PROJECT_ROOT=$(pwd)
PYTHON_EXECUTABLE=$(which python)
CMAKE_ARGS="-DCMAKE_BUILD_TYPE=Release -DPYTHON_BINDINGS=ON -DTORPP=OFF -DSO_TEST=OFF"
$PYTHON_EXECUTABLE -m pip install --upgrade pip setuptools twine wheel pybind11-stubgen

if [ ! -d "${BUILD_DIR}" ]; then
  mkdir ${BUILD_DIR}
fi
cd ${BUILD_DIR}

cmake ${CMAKE_ARGS} ..
cmake --build .

$PYTHON_EXECUTABLE -m pybind11_stubgen ${PROJECT}
$PYTHON_EXECUTABLE setup.py sdist bdist_wheel
