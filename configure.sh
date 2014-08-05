#!/bin/bash

set -e

[ -d ./build ] || mkdir ./build

if [ "$1" == "clean" ]; then
	rm -rf build/*
fi

cd build
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug 
cd ..
