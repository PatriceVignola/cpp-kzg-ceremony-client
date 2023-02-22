cmake_minimum_required(VERSION 3.10)
project(cpp-kzg-ceremony-client)

# For cross compiling
if("$ENV{CXX}" MATCHES "arm-linux-gnueabihf-g\\+\\+$")
  set(CMAKE_SYSTEM_PROCESSOR "arm")
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing")
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(dependencies.cmake)
add_subdirectory(src)
