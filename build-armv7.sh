#!/bin/sh
CXX=arm-linux-gnueabihf-g++ CC=arm-linux-gnueabihf-gcc cmake -S . -B build/ -DCMAKE_BUILD_TYPE=Release
CXX=arm-linux-gnueabihf-g++ CC=arm-linux-gnueabihf-gcc cmake --build build/