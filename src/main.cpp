#include "include/airgapped_flow.hpp"
#include "include/arg_parser.hpp"
#include "include/complete_flow.hpp"
#include "include/internet_flow.hpp"
#include <iostream>

int main(int argc, char** argv) {
  try {
    const ArgParser arg_parser(argc, argv);
    if (arg_parser.get_help_wanted()) {
      std::cout << arg_parser.get_help_message() << std::endl;
      return 0;
    }

    switch (arg_parser.get_client_mode()) {
    case ClientMode::Complete:
      complete_flow::launch(arg_parser);
      break;
    case ClientMode::Airgapped:
      airgapped_flow::launch(arg_parser);
      break;
    case ClientMode::Internet:
      internet_flow::launch(arg_parser);
      break;
    default:
      throw std::runtime_error("unsupported client mode");
    }
  } catch (const std::exception& ex) {
    std::cout << ex.what() << std::endl;
    return 1;
  }

  return 0;
}