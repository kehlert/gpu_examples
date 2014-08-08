#!/bin/bash

set -e

[ -d ./build ] || mkdir ./build

if [ "$1" == "clean" ]; then
	rm -rf build/*
fi

cd build
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug 
cd ..

#generate ctags
gmockDir="./dependencies/gmock-1.7.0"
gmockInclude="${gmockDir}/include/gmock/gmock.h"
gtestInclude="${gmockDir}/gtest/include/gtest/gtest.h"
ctags -R --c++-kinds=+p --fields=+liaS --extra=+q $gmockInclude $gtestInclude ./include/gpu_examples/
