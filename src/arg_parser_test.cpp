#include "include/arg_parser.hpp"
#include "include/ascii_title.hpp"
#include <absl/strings/str_cat.h>
#include <array>
#include <cxxopts.hpp>
#include <gtest/gtest.h>

static constexpr const char* usage_message = R"(
Usage:
  foo [OPTION...]

  -s, --sequencer arg  URL of the sequencer to use (default: 
                       https://seq.ceremony.ethereum.org)
  -a, --auth arg       Authentication provider to use. Choices: [ethereum, 
                       github] (default: ethereum)
  -e, --entropy arg    Type of entropy to use for the first layer. 
                       Additional CSPRNG entropy will be applied on top of 
                       it. Choices: [stdin] (default: stdin)
  -n, --no-signing     Disable the signing of the contribution. Although 
                       signing contributions is not mandatory, it is 
                       recommended to verify that the contributions listed 
                       in the transcript are actually generated by the 
                       addresses listed.
  -p, --port arg       Port to run the authentication server on. If not 
                       provided, a random port will be chosen.
  -h, --help           Print usage
)";

// NOLINTNEXTLINE
TEST(TestArgParser, ThrowsErrorUnknownOption) {
  const std::array<const char*, 2> args = {
      "foo",
      "--bar",
  };

  std::string expected_error;
#ifdef _WIN32
  absl::StrAppend(&expected_error,
                  "error when parsing arguments: Option 'bar' does not exist");
#else
  absl::StrAppend(&expected_error,
                  "error when parsing arguments: Option ‘bar’ does not exist");
#endif
  absl::StrAppend(&expected_error, "\n", ascii_title, usage_message);

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          ArgParser arg_parser(args.size(), args.data());
        } catch (const std::runtime_error& error) {
          EXPECT_EQ(expected_error, error.what());
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST(TestArgParser, ThrowsErrorInvalidAuthenticationProvider) {
  const std::array<const char*, 2> args = {
      "foo",
      "--auth=bar",
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
TEST(TestArgParser, ThrowsErrorInvalidEntropyType) {
  const std::array<const char*, 2> args = {
      "foo",
      "--entropy=bar",
  };

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          ArgParser arg_parser(args.size(), args.data());
        } catch (const std::runtime_error& error) {
          EXPECT_STREQ("invalid entropy type `bar`", error.what());
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST(TestArgParser, ThrowsExceptionShortArgsEqualDelimiter) {
  const std::array<const char*, 2> args = {
      "foo",
      "-s=bar_sequencer",
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
TEST(TestArgParser, HelpRequested) {
  const std::array<const char*, 2> args = {"foo", "--help"};
  ArgParser arg_parser(args.size(), args.data());
  EXPECT_TRUE(arg_parser.get_help_wanted());

  auto expected_help_message = absl::StrCat(ascii_title, usage_message);
  EXPECT_EQ(expected_help_message, arg_parser.get_help_message());
}

// NOLINTNEXTLINE
TEST(TestArgParser, ValidDefaultArgs) {
  const std::array<const char*, 1> args = {"foo"};
  ArgParser arg_parser(args.size(), args.data());
  EXPECT_EQ(AuthProvider::Ethereum, arg_parser.get_auth_provider());
  EXPECT_EQ("https://seq.ceremony.ethereum.org",
            arg_parser.get_sequencer_url());
  EXPECT_FALSE(arg_parser.get_help_wanted());
}

// NOLINTNEXTLINE
TEST(TestArgParser, ValidArgsEqualDelimiter) {
  const std::array<const char*, 3> args = {
      "foo",
      "--sequencer=foo_sequencer",
      "--auth=github",
  };
  ArgParser arg_parser(args.size(), args.data());
  EXPECT_EQ(AuthProvider::GitHub, arg_parser.get_auth_provider());
  EXPECT_EQ("foo_sequencer", arg_parser.get_sequencer_url());
  EXPECT_FALSE(arg_parser.get_help_wanted());
}

// NOLINTNEXTLINE
TEST(TestArgParser, ValidArgsSpaceDelimiter) {
  const std::array<const char*, 5> args = {
      "foo", "--sequencer", "foo_sequencer", "--auth", "github",
  };
  ArgParser arg_parser(args.size(), args.data());
  EXPECT_EQ(AuthProvider::GitHub, arg_parser.get_auth_provider());
  EXPECT_EQ("foo_sequencer", arg_parser.get_sequencer_url());
  EXPECT_FALSE(arg_parser.get_help_wanted());
}

// NOLINTNEXTLINE
TEST(TestArgParser, ValidShortArgsSpaceDelimiter) {
  const std::array<const char*, 5> args = {
      "foo", "-s", "foo_sequencer", "-a", "github",
  };
  ArgParser arg_parser(args.size(), args.data());
  EXPECT_EQ(AuthProvider::GitHub, arg_parser.get_auth_provider());
  EXPECT_EQ("foo_sequencer", arg_parser.get_sequencer_url());
  EXPECT_FALSE(arg_parser.get_help_wanted());
}

// NOLINTNEXTLINE
TEST(TestArgParser, GithubAuthenticationProvider) {
  const std::array<const char*, 1> args = {"foo"};
  ArgParser arg_parser(args.size(), args.data());
  EXPECT_EQ(AuthProvider::Ethereum, arg_parser.get_auth_provider());
}

// NOLINTNEXTLINE
TEST(TestArgParser, EthereumAuthenticationProvider) {
  const std::array<const char*, 1> args = {"foo"};
  ArgParser arg_parser(args.size(), args.data());
  EXPECT_EQ(AuthProvider::Ethereum, arg_parser.get_auth_provider());
}
