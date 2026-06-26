#!/usr/bin/env bash
set -e

# Clear output artifacts, keeping gcm.cache untouched
rm -f *.o *.out

# Leverage your dynamic standard library module caching trick
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

echo "[Pipeline] System module cache ready."

# Step 1: Compile the fcntl interface unit
g++ -std=c++26 -fmodules-ts -O3 -c modularFcntl.cppm -o modularFcntl.o

# Step 2: Compile and link the test binary
g++ -std=c++26 -fmodules-ts -O3 main.cpp modularFcntl.o -o test_fcntl.out

echo "Build complete. Run with: ./test_fcntl.out"
