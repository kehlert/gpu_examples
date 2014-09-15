#!/bin/bash

set -e

usage() { echo "usage: $0 [-c] [-b build_type]"; exit 1; }

validBuildTypes=("DEBUG" "RELWITHDEBINFO")

while getopts "b:c" opt; do
    case "${opt}" in
        c)
            echo -e "-----\nCleaning the build directory."
            rm -rf ./build/*
            ;;
        b)
            valid=false
            for validType in "${validBuildTypes[@]}"; do
                if [ "$OPTARG" == "$validType" ]; then
                    valid=true; break;
                fi
            done

            if ! $valid; then
                echo -n "ERROR: Build type not valid. Must be one of these: " 1>&2
                for type in "${validBuildTypes[@]}"; do echo -n "$type " 1>&2; done
                echo "" 1>&2;
                exit 1
            fi

            buildType="${OPTARG}"
            ;;
        *)
            usage
            ;;
    esac
done

if [ -z "$buildType" ]; then
    buildType="DEBUG"
fi

[ -d ./build ] || mkdir ./build

echo -e "-----\nbuild type: $buildType\n-----"

cd build
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="${buildType}"
cd ..

#generate ctags
gmockDir="./dependencies/gmock-1.7.0"
gmockInclude="${gmockDir}/include/gmock/gmock.h"
gtestInclude="${gmockDir}/gtest/include/gtest/gtest.h"
ctags -R --c++-kinds=+p --fields=+liaS --extra=+q $gmockInclude $gtestInclude ./include/gpu_examples/
