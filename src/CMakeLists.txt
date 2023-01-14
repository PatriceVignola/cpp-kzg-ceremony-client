add_executable(
  cpp-kzg-ceremony-client
  arg_parser.cc
  auth_callback_server.cc
  auth_request_link_response.cc
  contribution_response.cc
  hex_util.cc
  main.cc
  powers_of_tau.cc
  sequencer_client.cc)

add_dependencies(cpp-kzg-ceremony-client libblst)

target_include_directories(
  cpp-kzg-ceremony-client
  PRIVATE ${CMAKE_SOURCE_DIR}
          ${cxxopts_SOURCE_DIR}/include
          ${restbed_SOURCE_DIR}/source
          ${json_SOURCE_DIR}/include
          ${cpr_SOURCE_DIR}/include
          ${curl_SOURCE_DIR}/include
          ${valijson_SOURCE_DIR}/include
          ${blst_SOURCE_DIR}/bindings)

target_link_libraries(
  cpp-kzg-ceremony-client
  PRIVATE restbed-static
          nlohmann_json
          ${SSL_LIB}
          ${CRYPTO_LIB}
          cpr::cpr
          valijson
          ${LIB_BLST_PATH})
