#include "include/arg_parser.hpp"
#include "include/ascii_title.hpp"
#include "include/auth_browser.hpp"
#include "include/auth_info.hpp"
#include "include/auth_request_link.hpp"
#include "include/bls_signature.hpp"
#include "include/identity_fetcher.hpp"
#include "include/port_picker.hpp"
#include "include/secret_generator.hpp"
#include "include/sequencer_client.hpp"
#include "include/server.hpp"
#include "include/signing_browser.hpp"
#include <absl/strings/str_cat.h>
#include <cpr/cpr.h>
#include <iostream>

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
int main(int argc, char** argv) {
  try {
    const ArgParser arg_parser(argc, argv);
    if (arg_parser.get_help_wanted()) {
      std::cout << arg_parser.get_help_message() << std::endl;
      return 0;
    }
    // TODO: Launch the flows
  } catch (const std::exception& ex) {
    std::cout << ex.what() << std::endl;
    return 1;
  }

  return 0;
}