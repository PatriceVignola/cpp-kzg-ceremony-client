# Product static lib
add_library(
  product_static STATIC
  arg_parser.cpp
  auth_browser.cpp
  auth_callback_server.cpp
  auth_request_link.cpp
  batch_contribution.cpp
  contribution.cpp
  contribution_error.cpp
  contribution_receipt.cpp
  hex_util.cpp
  id_token.cpp
  port_picker.cpp
  power.cpp
  powers_of_tau.cpp
  receipt.cpp
  secret_generator.cpp
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
         ${blst_SOURCE_DIR}/bindings
         ${uint256_t_SOURCE_DIR}
         ${uint256_t_SOURCE_DIR}/uint128_t
         ${csprng_SOURCE_DIR}/source)

target_link_libraries(
  product_static
  PUBLIC restbed-static
         nlohmann_json
         ${SSL_LIB}
         ${CRYPTO_LIB}
         cpr::cpr
         valijson
         ${LIB_BLST_PATH}
         uint256_t
         csprng)

# Product executable
add_executable(cpp-kzg-ceremony-client main.cpp)
target_link_libraries(cpp-kzg-ceremony-client PRIVATE product_static)

# Test executable
add_executable(
  cpp-kzg-ceremony-client-test
  arg_parser_test.cpp auth_callback_server_test.cpp auth_request_link_test.cpp)
target_link_libraries(cpp-kzg-ceremony-client-test PRIVATE product_static
                                                           gtest_static)
target_include_directories(cpp-kzg-ceremony-client-test
                           PRIVATE ${googletest_SOURCE_DIR}/googletest/include)

set_target_properties(
  cpp-kzg-ceremony-client-test
  PROPERTIES SKIP_BUILD_RPATH FALSE
             BUILD_WITH_INSTALL_RPATH TRUE
             INSTALL_RPATH "$\{ORIGIN\}")
