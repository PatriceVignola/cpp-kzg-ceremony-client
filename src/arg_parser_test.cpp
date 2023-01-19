#include "include/arg_parser.hpp"
#include "include/ascii_title.hpp"
#include <array>
#include <cxxopts.hpp>
#include <gtest/gtest.h>
#include <sstream>

static constexpr const char* usage_message = R"(
Usage:
  foo [OPTION...]

  -s, --sequencer arg  URL of the sequencer to use (default: 
                       https://seq.ceremony.ethereum.org)
  -p, --port arg       Local port to use for the authentication callback 
                       (default: 8080)
  -a, --auth arg       Authentication provider to use. Choices: [ethereum, 
                       github] (default: ethereum)
  -e, --entropy arg    A phrase to initialize the entropy. Can be any 
                       length, but will be truncated or padded to 256 
                       characters depending on the length. This phrase will 
                       be fed more randomness before a secret is generated, 
                       so it does not already need to random.
  -h, --help           Print usage
)";

// NOLINTNEXTLINE
TEST(TestArgParser, ThrowsErrorUnknownOption) {
  const std::array<const char*, 3> args = {
      "foo",
      "--bar",
      "--entropy=foo",
  };

  std::stringstream expected_error;
#ifdef _WIN32
  expected_error << "error when parsing arguments: Option 'bar' does not exist";
#else
  expected_error << "error when parsing arguments: Option ‘bar’ does not exist";
#endif
  expected_error << std::endl << ascii_title << usage_message;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          ArgParser arg_parser(args.size(), args.data());
        } catch (const std::runtime_error& error) {
          EXPECT_EQ(expected_error.str(), error.what());
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST(TestArgParser, ThrowsErrorInvalidAuthenticationProvider) {
  const std::array<const char*, 3> args = {
      "foo",
      "--auth=bar",
      "--entropy=foo",
  };

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          ArgParser arg_parser(args.size(), args.data());
        } catch (const std::runtime_error& error) {
          EXPECT_STREQ("invalid authentication provider `bar`", error.what());
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST(TestArgParser, ThrowsErrorInvalidPort) {
  const std::array<const char*, 3> args = {
      "foo",
      "--port=bar",
      "--entropy=foo",
  };

#ifdef _WIN32
  const auto* expected_error = "Argument 'bar' failed to parse";
#else
  const auto* expected_error = "Argument ‘bar’ failed to parse";
#endif

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          ArgParser arg_parser(args.size(), args.data());
        } catch (const cxxopts::argument_incorrect_type& error) {
          EXPECT_STREQ(expected_error, error.what());
          throw;
        }
      },
      cxxopts::argument_incorrect_type);
}

// NOLINTNEXTLINE
TEST(TestArgParser, ThrowsExceptionShortArgsEqualDelimiter) {
  const std::array<const char*, 3> args = {
      "foo",
      "-s=bar_sequencer",
      "--entropy=foo",
  };

#ifdef _WIN32
  const auto* expected_error =
      "Argument '-s=bar_sequencer' starts with a - but has "
      "incorrect syntax";
#else
  const auto* expected_error =
      "Argument ‘-s=bar_sequencer’ starts with a - but has "
      "incorrect syntax";
#endif

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          ArgParser arg_parser(args.size(), args.data());
        } catch (const cxxopts::option_syntax_exception& error) {
          EXPECT_STREQ(expected_error, error.what());
          throw;
        }
      },
      cxxopts::option_syntax_exception);
}

// NOLINTNEXTLINE
TEST(TestArgParser, ThrowsExceptionMissingEntropy) {
  const std::array<const char*, 1> args = {"foo"};

#ifdef _WIN32
  const auto* expected_error = "Option 'entropy' has no value";
#else
  const auto* expected_error = "Option ‘entropy’ has no value";
#endif

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          ArgParser arg_parser(args.size(), args.data());
        } catch (const cxxopts::option_has_no_value_exception& error) {
          EXPECT_STREQ(expected_error, error.what());
          throw;
        }
      },
      cxxopts::option_has_no_value_exception);
}

// NOLINTNEXTLINE
TEST(TestArgParser, HelpRequested) {
  const std::array<const char*, 2> args = {"foo", "--help"};
  ArgParser arg_parser(args.size(), args.data());
  EXPECT_TRUE(arg_parser.get_help_wanted());

  std::stringstream expected_help_message;
  expected_help_message << ascii_title << usage_message;

  EXPECT_EQ(expected_help_message.str(), arg_parser.get_help_message());
}

// NOLINTNEXTLINE
TEST(TestArgParser, ValidDefaultArgs) {
  const std::array<const char*, 2> args = {"foo", "--entropy=bar"};
  ArgParser arg_parser(args.size(), args.data());
  EXPECT_EQ(AuthProvider::Ethereum, arg_parser.get_auth_provider());
  EXPECT_EQ(8080, arg_parser.get_auth_callback_port());
  EXPECT_EQ("bar", arg_parser.get_entropy());
  EXPECT_EQ("https://seq.ceremony.ethereum.org",
            arg_parser.get_sequencer_url());
  EXPECT_FALSE(arg_parser.get_help_wanted());
}

// NOLINTNEXTLINE
TEST(TestArgParser, ValidArgsEqualDelimiter) {
  const std::array<const char*, 5> args = {
      "foo",           "--sequencer=foo_sequencer",
      "--port=1234",   "--auth=github",
      "--entropy=bar",
  };
  ArgParser arg_parser(args.size(), args.data());
  EXPECT_EQ(AuthProvider::GitHub, arg_parser.get_auth_provider());
  EXPECT_EQ("foo_sequencer", arg_parser.get_sequencer_url());
  EXPECT_EQ(1234, arg_parser.get_auth_callback_port());
  EXPECT_EQ("bar", arg_parser.get_entropy());
  EXPECT_FALSE(arg_parser.get_help_wanted());
}

// NOLINTNEXTLINE
TEST(TestArgParser, ValidArgsSpaceDelimiter) {
  const std::array<const char*, 9> args = {
      "foo",    "--sequencer", "foo_sequencer", "--port", "1234",
      "--auth", "github",      "--entropy",     "bar",
  };
  ArgParser arg_parser(args.size(), args.data());
  EXPECT_EQ(AuthProvider::GitHub, arg_parser.get_auth_provider());
  EXPECT_EQ("foo_sequencer", arg_parser.get_sequencer_url());
  EXPECT_EQ(1234, arg_parser.get_auth_callback_port());
  EXPECT_EQ("bar", arg_parser.get_entropy());
  EXPECT_FALSE(arg_parser.get_help_wanted());
}

// NOLINTNEXTLINE
TEST(TestArgParser, ValidShortArgsSpaceDelimiter) {
  const std::array<const char*, 9> args = {
      "foo", "-s", "foo_sequencer", "-p", "1234", "-a", "github", "-e", "bar",
  };
  ArgParser arg_parser(args.size(), args.data());
  EXPECT_EQ(AuthProvider::GitHub, arg_parser.get_auth_provider());
  EXPECT_EQ("foo_sequencer", arg_parser.get_sequencer_url());
  EXPECT_EQ(1234, arg_parser.get_auth_callback_port());
  EXPECT_EQ("bar", arg_parser.get_entropy());
  EXPECT_FALSE(arg_parser.get_help_wanted());
}

// NOLINTNEXTLINE
TEST(TestArgParser, GithubAuthenticationProvider) {
  const std::array<const char*, 2> args = {"foo", "--entropy=bar"};
  ArgParser arg_parser(args.size(), args.data());
  EXPECT_EQ(AuthProvider::Ethereum, arg_parser.get_auth_provider());
}

// NOLINTNEXTLINE
TEST(TestArgParser, EthereumAuthenticationProvider) {
  const std::array<const char*, 2> args = {"foo", "--entropy=bar"};
  ArgParser arg_parser(args.size(), args.data());
  EXPECT_EQ(AuthProvider::Ethereum, arg_parser.get_auth_provider());
}
