set(CMAKE_FOLDER external)

include(FetchContent)

# These options are defined in the restbed library, which we don't control
# cmake-lint: disable=C0103
set(BUILD_SSL
    OFF
    CACHE INTERNAL "")
set(BUILD_TESTS
    OFF
    CACHE INTERNAL "")

# These options are defined in the cpr library, which we don't control
# cmake-lint: disable=C0103
set(BUILD_SHARED_LIBS
    OFF
    CACHE INTERNAL "")

# cxxopts Library
FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts
  GIT_TAG v3.0.0)

# Restbed Library
FetchContent_Declare(
  restbed
  GIT_REPOSITORY https://github.com/Corvusoft/restbed
  GIT_TAG 4.8)

# CPR Library
FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr
  GIT_TAG 1.9.3)

# JSON Library
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG v3.11.2)

# Valijson Library
FetchContent_Declare(
  valijson
  GIT_REPOSITORY https://github.com/tristanpenman/valijson
  GIT_TAG v1.0)

# BLST Library
FetchContent_Declare(
  blst
  GIT_REPOSITORY https://github.com/supranational/blst
  GIT_TAG v0.3.10)

# BLST Library
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG release-1.12.1)

# uint256_t Library
FetchContent_Declare(
  uint256_t
  GIT_REPOSITORY https://github.com/calccrypto/uint256_t
  GIT_TAG 092e4c82e2b607da19ea4bae94f857756ce07d81)

# csprng Library
FetchContent_Declare(
  csprng
  GIT_REPOSITORY https://github.com/Duthomhas/CSPRNG
  GIT_TAG 8768a94b4b04213c0798b80824a04ae4990e9847)

# Download and extract dependencies.
FetchContent_MakeAvailable(
  cxxopts
  restbed
  cpr
  json
  valijson
  blst
  googletest
  uint256_t)

# csprng has a broken CMakeLists.txt file, so manaually download it instead
FetchContent_Populate(csprng)

if(WIN32)
  set(LIB_BLST_PATH ${blst_BINARY_DIR}/blst.lib)
  set(BLST_BUILD_SCRIPT_PATH ${blst_SOURCE_DIR}/build.bat)
else()
  set(LIB_BLST_PATH ${blst_BINARY_DIR}/libblst.a)
  set(BLST_BUILD_SCRIPT_PATH ${blst_SOURCE_DIR}/build.sh)
endif(WIN32)

add_custom_command(
  OUTPUT ${LIB_BLST_PATH}
  WORKING_DIRECTORY ${blst_BINARY_DIR}
  COMMAND ${BLST_BUILD_SCRIPT_PATH} -D__BLST_PORTABLE__
  COMMENT "Building libblst")

add_custom_target(
  libblst
  DEPENDS ${LIB_BLST_PATH}
  COMMENT "Creating libblst target")

add_library(
  gtest_static STATIC
  ${googletest_SOURCE_DIR}/googletest/src/gtest-all.cc
  ${googletest_SOURCE_DIR}/googletest/src/gtest_main.cc
  ${googletest_SOURCE_DIR}/googlemock/src/gmock-all.cc
  ${googletest_SOURCE_DIR}/googlemock/src/gmock_main.cc)

target_include_directories(
  gtest_static
  PRIVATE ${googletest_SOURCE_DIR}/googletest
          ${googletest_SOURCE_DIR}/googletest/include
          ${googletest_SOURCE_DIR}/googlemock
          ${googletest_SOURCE_DIR}/googlemock/include)

# Build the uint256_t library
add_library(uint256_t STATIC ${uint256_t_SOURCE_DIR}/uint256_t.cpp
                             ${uint256_t_SOURCE_DIR}/uint128_t/uint128_t.cpp)
target_include_directories(uint256_t PRIVATE ${uint256_t_SOURCE_DIR}
                                             ${uint256_t_SOURCE_DIR}/uint128_t)

# Build the cspring library
add_library(csprng STATIC ${csprng_SOURCE_DIR}/source/csprng.cpp)
target_include_directories(csprng PRIVATE ${csprng_SOURCE_DIR}/source)
