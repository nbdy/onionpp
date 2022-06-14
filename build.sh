#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Usage: ./build.sh {toolchain name}"
  echo "See https://github.com/dockcross/dockcross#summary-cross-compilers"
  exit
fi

if [ ! -d build ]; then mkdir build; fi
docker run --rm -v build:/work/build -v $PWD:/work/:ro dockcross/$1 bash -c "cmake -Bbuild -H."
