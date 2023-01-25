#include "include/contribution_error.hpp"
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

// NOLINTNEXTLINE
TEST(TestContributionError, ThrowsErrorIfCodeNotFound) {
  static const auto contribution_error_json = R"(
    {
      "error": "foo"
    }
  )"_json;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          contribution_error_json.get<ContributionError>();
        } catch (
            const nlohmann::json_abi_v3_11_2::detail::out_of_range& error) {
          EXPECT_STREQ("[json.exception.out_of_range.403] key 'code' not found",
                       error.what());
          throw;
        }
      },
      nlohmann::json_abi_v3_11_2::detail::out_of_range);
}

// NOLINTNEXTLINE
TEST(TestContributionError, ThrowsErrorIfErrorNotFound) {
  static const auto contribution_error_json = R"(
    {
      "code": "foo"
    }
  )"_json;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          contribution_error_json.get<ContributionError>();
        } catch (
            const nlohmann::json_abi_v3_11_2::detail::out_of_range& error) {
          EXPECT_STREQ(
              "[json.exception.out_of_range.403] key 'error' not found",
              error.what());
          throw;
        }
      },
      nlohmann::json_abi_v3_11_2::detail::out_of_range);
}

// NOLINTNEXTLINE
TEST(TestContributionError, ParsesJsonCorrectly) {
  static const auto contribution_error_json = R"(
    {
      "code": "foo",
      "error": "bar"
    }
  )"_json;

  const auto contribution_error =
      contribution_error_json.get<ContributionError>();

  EXPECT_EQ("foo", contribution_error.get_code());
  EXPECT_EQ("bar", contribution_error.get_error());
}