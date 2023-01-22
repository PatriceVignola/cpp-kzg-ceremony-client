#include "include/arg_parser.hpp"
#include "include/ascii_title.hpp"
#include <cassert>
#include <cxxopts.hpp>
#include <sstream>

ArgParser::ArgParser(int argc, const char* const* argv) {
  assert(argc > 0);

  try {
    cxxopts::Options options(*argv, ascii_title);

    options.add_option("", "s", "sequencer", "URL of the sequencer to use",
                       cxxopts::value<std::string>()->default_value(
                           "https://seq.ceremony.ethereum.org"),
                       "");

    options.add_option(
        "", "a", "auth",
        "Authentication provider to use. Choices: [ethereum, github]",
        cxxopts::value<std::string>()->default_value("ethereum"), "");

    options.add_option(
        "", "e", "entropy",
        "A phrase to initialize the entropy. Can be any length, but will "
        "be truncated or padded to 256 characters depending on the length. "
        "This phrase will be fed more randomness before a secret is "
        "generated, so it does not already need to random.",
        cxxopts::value<std::string>(), "");

    options.add_option("", "h", "help", "Print usage", cxxopts::value<bool>(),
                       "");

    help_message_ = options.help();
    auto parse_result = options.parse(argc, argv);

    help_wanted_ = parse_result.count("help") > 0;
    if (!help_wanted_) {
      sequencer_url_ = parse_result["sequencer"].as<std::string>();
      entropy_ = parse_result["entropy"].as<std::string>();
    }

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
