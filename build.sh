#!/usr/bin/env bash
set -e
# import std compilation
if [ ! -d "gcm.cache" ]; then
    echo "Building standard library module cache..."
    GCC_VERSION=$(g++ -dumpversion)
    STD_MODULE_PATH="/usr/include/c++/${GCC_VERSION}/bits/std.cc"
    
    if [ -f "${STD_MODULE_PATH}" ]; then
        g++ -std=c++26 -fmodules-ts -O3 -c "${STD_MODULE_PATH}"
    else
        echo "Error: Could not locate system std.cc file automatically."
        exit 1
    fi
fi

g++ -std=c++26 -fmodules-ts -O3 -c modularFcntl.cppm -o modularFcntl.o

echo "done. now go code"
