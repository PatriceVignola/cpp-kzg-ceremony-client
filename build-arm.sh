#!/bin/sh

CXX=arm-linux-gnueabi-g++ CC=arm-linux-gnueabi-gcc cmake -S . -B build/ -DCMAKE_BUILD_TYPE=Release
CXX=arm-linux-gnueabi-g++ CC=arm-linux-gnueabi-gcc cmake --build build/