#include "include/auth_request_link.hpp"
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

// NOLINTNEXTLINE
TEST(TestAuthRequestLink, ThrowsErrorIfEthereumLinkNotFound) {
  static const auto auth_request_link_json = R"(
    {
        "github_auth_url": "foo"
    }
  )"_json;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          auth_request_link_json.get<AuthRequestLink>();
        } catch (
            const nlohmann::json_abi_v3_11_2::detail::out_of_range& error) {
          EXPECT_STREQ(
              "[json.exception.out_of_range.403] key 'eth_auth_url' not found",
              error.what());
          throw;
        }
      },
      nlohmann::json_abi_v3_11_2::detail::out_of_range);
}

// NOLINTNEXTLINE
TEST(TestAuthRequestLink, ThrowsErrorIfGithubLinkNotFound) {
  static const auto auth_request_link_json = R"(
    {
        "eth_auth_url": "foo"
    }
  )"_json;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          auth_request_link_json.get<AuthRequestLink>();
        } catch (
            const nlohmann::json_abi_v3_11_2::detail::out_of_range& error) {
          EXPECT_STREQ("[json.exception.out_of_range.403] key "
                       "'github_auth_url' not found",
                       error.what());
          throw;
        }
      },
      nlohmann::json_abi_v3_11_2::detail::out_of_range);
}

// NOLINTNEXTLINE
TEST(TestAuthRequestLink, ThrowsErrorIfEthereumLinkIsNotString) {
  static const auto auth_request_link_json = R"(
    {
        "eth_auth_url": 1234,
        "github_auth_url": "foo"
    }
  )"_json;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          auth_request_link_json.get<AuthRequestLink>();
        } catch (const nlohmann::json_abi_v3_11_2::detail::type_error& error) {
          EXPECT_STREQ("[json.exception.type_error.302] type must be string, "
                       "but is number",
                       error.what());
          throw;
        }
      },
      nlohmann::json_abi_v3_11_2::detail::type_error);
}

// NOLINTNEXTLINE
TEST(TestAuthRequestLink, ThrowsErrorIfGithubLinkIsNotString) {
  static const auto auth_request_link_json = R"(
    {
        "eth_auth_url": "foo",
        "github_auth_url": 1234
    }
  )"_json;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          auth_request_link_json.get<AuthRequestLink>();
        } catch (const nlohmann::json_abi_v3_11_2::detail::type_error& error) {
          EXPECT_STREQ("[json.exception.type_error.302] type must be string, "
                       "but is number",
                       error.what());
          throw;
        }
      },
      nlohmann::json_abi_v3_11_2::detail::type_error);
}

// NOLINTNEXTLINE
TEST(TestAuthRequestLink, CorrectlyConvertsValidJson) {
  static const auto auth_request_link_json = R"(
    {
        "eth_auth_url": "foo",
        "github_auth_url": "bar"
    }
  )"_json;

  const auto auth_request_link = auth_request_link_json.get<AuthRequestLink>();
  EXPECT_EQ("foo", auth_request_link.get_eth_auth_url());
  EXPECT_EQ("bar", auth_request_link.get_github_auth_url());
}
