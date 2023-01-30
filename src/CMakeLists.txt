# Product static lib
add_library(
  product_static STATIC
  arg_parser.cpp
  auth_browser.cpp
  auth_request_link.cpp
  batch_contribution.cpp
  batch_transcript.cpp
  bls_signature.cpp
  bls_signer.cpp
  contribution.cpp
  contribution_error.cpp
  contribution_receipt.cpp
  github_user.cpp
  hex_util.cpp
  id_token.cpp
  identity_fetcher.cpp
  port_picker.cpp
  pot_pubkey_message.cpp
  power.cpp
  powers_of_tau.cpp
  receipt.cpp
  sequencer_client.cpp
  server.cpp
  signing_browser.cpp
  transcript.cpp
  witness.cpp)

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
  arg_parser_test.cpp
  auth_request_link_test.cpp
  batch_contribution_test.cpp
  contribution_error_test.cpp
  contribution_receipt_test.cpp
  hex_util_test.cpp
  receipt_test.cpp
  secret_generator_test.cpp
  server_test.cpp)

target_link_libraries(cpp-kzg-ceremony-client-test PRIVATE product_static
                                                           gtest_static)
target_include_directories(
  cpp-kzg-ceremony-client-test
  PRIVATE ${googletest_SOURCE_DIR}/googletest/include
          ${googletest_SOURCE_DIR}/googlemock/include)

set_target_properties(
  cpp-kzg-ceremony-client-test
  PROPERTIES SKIP_BUILD_RPATH FALSE
             BUILD_WITH_INSTALL_RPATH TRUE
             INSTALL_RPATH "$\{ORIGIN\}")

# Copy the test vectors to the binary folder
configure_file(${CMAKE_SOURCE_DIR}/test_data/initialContribution.json
               ${CMAKE_BINARY_DIR}/bin/initialContribution.json)

configure_file(${CMAKE_SOURCE_DIR}/test_data/updatedContribution.json
               ${CMAKE_BINARY_DIR}/bin/updatedContribution.json)
