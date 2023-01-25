cmake_minimum_required(VERSION 3.10)
project(cpp-kzg-ceremony-client)

# Avoid "illegal instruction" in BLST when running the executable on a machine
# that didn't build it
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__BLST_PORTABLE__")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(dependencies.cmake)
add_subdirectory(src)
