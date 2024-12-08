#!/bin/bash

# TODO: Iterate over list of all available images?
# https://github.com/dockcross/dockcross#summary-cross-compilers

./cross-build.sh linux-x64
./cross-build.sh linux-x86

./cross-build.sh windows-static-x64
./cross-build.sh windows-static-x86
