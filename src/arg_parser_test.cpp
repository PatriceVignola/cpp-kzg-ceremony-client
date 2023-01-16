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
  -h, --help           Print usage
)";

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST(TestArgParser, ThrowsErrorUnknownOption) {
  const std::array<const char*, 2> args = {"foo", "--bar"};

  EXPECT_THROW(
      {
        try {
          ArgParser arg_parser(args.size(), args.data());
        } catch (const std::runtime_error& error) {
          std::stringstream expected_error;

#ifdef _WIN32
          expected_error
              << "error when parsing arguments: Option 'bar' does not exist";
#else
          expected_error
              << "error when parsing arguments: Option ‘bar’ does not exist";
#endif
          expected_error << std::endl << ascii_title << usage_message;

          EXPECT_EQ(expected_error.str(), error.what());
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST(TestArgParser, ThrowsErrorInvalidAuthenticationProvider) {
  const std::array<const char*, 2> args = {"foo", "--auth=bar"};

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

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST(TestArgParser, ThrowsErrorInvalidPort) {
  const std::array<const char*, 2> args = {"foo", "--port=foo"};

  EXPECT_THROW(
      {
        try {
          ArgParser arg_parser(args.size(), args.data());
        } catch (const cxxopts::argument_incorrect_type& error) {
#ifdef _WIN32
          EXPECT_STREQ("Argument 'foo' failed to parse", error.what());
#else
          EXPECT_STREQ("Argument ‘foo’ failed to parse", error.what());
#endif
          throw;
        }
      },
      cxxopts::argument_incorrect_type);
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST(TestArgParser, ThrowsExceptionShortArgsEqualDelimiter) {
  const std::array<const char*, 2> args = {"foo", "-s=foo_sequencer"};

  EXPECT_THROW(
      {
        try {
          ArgParser arg_parser(args.size(), args.data());
        } catch (const cxxopts::option_syntax_exception& error) {
#ifdef _WIN32
          EXPECT_STREQ("Argument '-s=foo_sequencer' starts with a - but has "
                       "incorrect syntax",
                       error.what());
#else
          EXPECT_STREQ("Argument ‘-s=foo_sequencer’ starts with a - but has "
                       "incorrect syntax",
                       error.what());
#endif
          throw;
        }
      },
      cxxopts::option_syntax_exception);
}

TEST(TestArgParser, HelpRequested) {
  const std::array<const char*, 2> args = {"foo", "--help"};
  ArgParser arg_parser(args.size(), args.data());
  EXPECT_TRUE(arg_parser.get_help_wanted());

  std::stringstream expected_help_message;
  expected_help_message << ascii_title << usage_message;

  EXPECT_EQ(expected_help_message.str(), arg_parser.get_help_message());
}

TEST(TestArgParser, ValidDefaultArgs) {
  const std::array<const char*, 1> args = {"foo"};
  ArgParser arg_parser(args.size(), args.data());
  EXPECT_EQ(AuthProvider::Ethereum, arg_parser.get_auth_provider());
  EXPECT_EQ(8080, arg_parser.get_auth_callback_port());
  EXPECT_EQ("https://seq.ceremony.ethereum.org",
            arg_parser.get_sequencer_url());
  EXPECT_FALSE(arg_parser.get_help_wanted());
}

TEST(TestArgParser, ValidArgsEqualDelimiter) {
  const std::array<const char*, 4> args = {
      "foo",
      "--sequencer=foo_sequencer",
      "--port=1234",
      "--auth=github",
  };
  ArgParser arg_parser(args.size(), args.data());
  EXPECT_EQ(AuthProvider::GitHub, arg_parser.get_auth_provider());
  EXPECT_EQ("foo_sequencer", arg_parser.get_sequencer_url());
  EXPECT_EQ(1234, arg_parser.get_auth_callback_port());
  EXPECT_FALSE(arg_parser.get_help_wanted());
}

TEST(TestArgParser, ValidArgsSpaceDelimiter) {
  const std::array<const char*, 7> args = {
      "foo",  "--sequencer", "foo_sequencer", "--port",
      "1234", "--auth",      "github",
  };
  ArgParser arg_parser(args.size(), args.data());
  EXPECT_EQ(AuthProvider::GitHub, arg_parser.get_auth_provider());
  EXPECT_EQ("foo_sequencer", arg_parser.get_sequencer_url());
  EXPECT_EQ(1234, arg_parser.get_auth_callback_port());
  EXPECT_FALSE(arg_parser.get_help_wanted());
}

TEST(TestArgParser, ValidShortArgsSpaceDelimiter) {
  const std::array<const char*, 7> args = {"foo",  "-s", "foo_sequencer", "-p",
                                           "1234", "-a", "github"};
  ArgParser arg_parser(args.size(), args.data());
  EXPECT_EQ(AuthProvider::GitHub, arg_parser.get_auth_provider());
  EXPECT_EQ("foo_sequencer", arg_parser.get_sequencer_url());
  EXPECT_EQ(1234, arg_parser.get_auth_callback_port());
  EXPECT_FALSE(arg_parser.get_help_wanted());
}

TEST(TestArgParser, GithubAuthenticationProvider) {
  const std::array<const char*, 1> args = {"foo"};
  ArgParser arg_parser(args.size(), args.data());
  EXPECT_EQ(AuthProvider::Ethereum, arg_parser.get_auth_provider());
}

TEST(TestArgParser, EthereumAuthenticationProvider) {
  const std::array<const char*, 1> args = {"foo"};
  ArgParser arg_parser(args.size(), args.data());
  EXPECT_EQ(AuthProvider::Ethereum, arg_parser.get_auth_provider());
}