# Product static lib
add_library(
  product_static STATIC
  arg_parser.cpp
  auth_callback_server.cpp
  auth_request_link_response.cpp
  contribution_response.cpp
  hex_util.cpp
  powers_of_tau.cpp
  sequencer_client.cpp)

add_dependencies(product_static libblst)

target_include_directories(
  product_static
  PUBLIC ${CMAKE_SOURCE_DIR}
         ${cxxopts_SOURCE_DIR}/include
         ${restbed_SOURCE_DIR}/source
         ${json_SOURCE_DIR}/include
         ${cpr_SOURCE_DIR}/include
         ${curl_SOURCE_DIR}/include
         ${valijson_SOURCE_DIR}/include
         ${blst_SOURCE_DIR}/bindings)

target_link_libraries(
  product_static
  PUBLIC restbed-static
         nlohmann_json
         ${SSL_LIB}
         ${CRYPTO_LIB}
         cpr::cpr
         valijson
         ${LIB_BLST_PATH})

# Product executable
add_executable(cpp-kzg-ceremony-client main.cpp)
target_link_libraries(cpp-kzg-ceremony-client PRIVATE product_static)

# Test executable
add_executable(cpp-kzg-ceremony-client-test arg_parser_test.cpp)
target_link_libraries(cpp-kzg-ceremony-client-test PRIVATE product_static
                                                           gtest_static)
target_include_directories(cpp-kzg-ceremony-client-test
                           PRIVATE ${googletest_SOURCE_DIR}/googletest/include)

set_target_properties(
  cpp-kzg-ceremony-client-test
  PROPERTIES SKIP_BUILD_RPATH FALSE
             BUILD_WITH_INSTALL_RPATH TRUE
             INSTALL_RPATH "$\{ORIGIN\}")
