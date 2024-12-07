#!/bin/bash

mkdir -p build-default
cd build-default
cmake .. -DCMAKE_BUILD_TYPE=Relase
make -j$(nproc)
cd ..

mkdir -p build-windows
cd build-windows
cmake .. -DCMAKE_BUILD_TYPE=Relase -DWINDOWS=ON
make -j$(nproc)
cd ..

mkdir -p build-windows-x86_64
cd build-windows
cmake .. -DCMAKE_BUILD_TYPE=Relase -DWINDOWS=ON -Dx86_64
make -j$(nproc)
cd ..
