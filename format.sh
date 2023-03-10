#!/usr/bin/env bash

set -e

GREEN='\033[0;32m'
NO_COLOR='\033[0m'

# Run cmake-format
echo -e "${GREEN}Formatting cmake files...${NO_COLOR}"
find . -iname CMakeLists.txt -not \( -path "./build/*" -prune \) -o -iname "*.cmake" \
    -not \( -path "./build/*" -prune \) \
    | xargs cmake-format --in-place

# Run cmake-lint
echo -e "${GREEN}Linting cmake files...${NO_COLOR}"
find . -iname CMakeLists.txt -not \( -path "./build/*" -prune \) -o -iname "*.cmake" \
    -not \( -path "./build/*" -prune \) \
    | xargs cmake-lint

# Run clang-format
echo -e "${GREEN}Formatting C++ files...${NO_COLOR}"
find . -iname *.hpp -not \( -path "./build/*" -prune \) -o -iname *.cpp \
    -not \( -path "./build/*" -prune \) \
    | xargs clang-format -i

# Run clang-tidy
echo -e "${GREEN}Linting C++ files...${NO_COLOR}"
./clang-tidy.sh

echo -e "${GREEN}Done!${NO_COLOR}"
