#!/bin/sh

CXX=aarch64-linux-gnu-g++ CC=aarch64-linux-gnu-gcc cmake -S . -B build/ -DCMAKE_BUILD_TYPE=Release
CXX=aarch64-linux-gnu-g++ CC=aarch64-linux-gnu-gcc cmake --build build/