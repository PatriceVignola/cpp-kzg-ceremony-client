#!/usr/bin/env bash

set -e

find . -iname *.hpp -not \( -path "./build/*" -prune \) -o -iname *.cpp \
    -not \( -path "./build/*" -prune \) \
    | xargs clang-tidy -p build
