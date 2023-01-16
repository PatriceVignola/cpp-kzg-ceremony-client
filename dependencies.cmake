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

# cpp-httplib Library
FetchContent_Declare(
  httplib
  GIT_REPOSITORY https://github.com/yhirose/cpp-httplib
  GIT_TAG v0.11.4)

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

# Download and extract dependencies.
FetchContent_MakeAvailable(
  cxxopts
  restbed
  cpr
  json
  valijson
  blst
  googletest)

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
  COMMAND ${BLST_BUILD_SCRIPT_PATH}
  COMMENT "Building libblst")

add_custom_target(
  libblst
  DEPENDS ${LIB_BLST_PATH}
  COMMENT "Creating libblst target")
