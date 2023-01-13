#include "include/arg_parser.h"
#include "include/ascii_title.h"
#include <cxxopts.hpp>
#include <sstream>

ArgParser::ArgParser(int argc, char** argv) {
  try {
    cxxopts::Options options("cpp-kzg-ceremony-client", ascii_title);

    options.add_option("", "s", "sequencer", "URL of the sequencer to use",
                       cxxopts::value<std::string>()->default_value(
                           "https://seq.ceremony.ethereum.org"),
                       "");

    options.add_option("", "p", "port",
                       "Local port to use for the authentication callback",
                       cxxopts::value<uint16_t>()->default_value("8080"), "");

    options.add_option(
        "", "a", "auth",
        "Authentication provider to use. Choices: [ethereum, github]",
        cxxopts::value<std::string>()->default_value("ethereum"), "");

    options.add_option("", "h", "help", "Print usage", cxxopts::value<bool>(),
                       "");

    help_message_ = options.help();
    auto parse_result = options.parse(argc, argv);

    help_wanted_ = parse_result.count("help") > 0;
    port_ = parse_result["port"].as<uint16_t>();
    sequencer_url_ = parse_result["sequencer"].as<std::string>();

    const auto auth_provider = parse_result["auth"].as<std::string>();
    if (auth_provider == "ethereum") {
      auth_provider_ = AuthProvider::Ethereum;
    } else if (auth_provider == "github") {
      auth_provider_ = AuthProvider::GitHub;
    } else {
      std::stringstream error_stream;
      error_stream << "invalid authentication provider `" << auth_provider
                   << "`";
      throw std::runtime_error(error_stream.str());
    }
  } catch (const cxxopts::option_not_exists_exception& ex) {
    std::stringstream error_stream;
    error_stream << "error when parsing arguments: " << ex.what() << std::endl
                 << help_message_;
    throw std::runtime_error(error_stream.str());
  }
}