#include "include/receipt.hpp"
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

// NOLINTNEXTLINE
TEST(TestReceipt, ThrowsErrorIfIdTokenNotFound) {
  static const auto receipt_json = R"(
    {
      "g2": "foo"
    }
  )"_json;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          receipt_json.get<Receipt>();
        } catch (
            const nlohmann::json_abi_v3_11_2::detail::out_of_range& error) {
          EXPECT_STREQ(
              "[json.exception.out_of_range.403] key 'id_token' not found",
              error.what());
          throw;
        }
      },
      nlohmann::json_abi_v3_11_2::detail::out_of_range);
}

// NOLINTNEXTLINE
TEST(TestReceipt, ThrowsErrorIfG2NotFound) {
  static const auto receipt_json = R"(
    {
      "id_token": {
        "sub": "dummy_sub",
        "nickname": "dummy_nickname",
        "provider": "dummy_provider",
        "exp": 1234
      }
    }
  )"_json;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          receipt_json.get<Receipt>();
        } catch (
            const nlohmann::json_abi_v3_11_2::detail::out_of_range& error) {
          EXPECT_STREQ("[json.exception.out_of_range.403] key 'g2' not found",
                       error.what());
          throw;
        }
      },
      nlohmann::json_abi_v3_11_2::detail::out_of_range);
}

// NOLINTNEXTLINE
TEST(TestReceipt, ParsesJsonCorrectly) {
  static const auto receipt_json = R"(
    {
      "g2": "dummy_g2",
      "id_token": {
        "sub": "dummy_sub",
        "nickname": "dummy_nickname",
        "provider": "dummy_provider",
        "exp": 1234
      }
    }
  )"_json;

  const auto receipt = receipt_json.get<Receipt>();
  const auto& id_token = receipt.get_id_token();

  EXPECT_EQ("dummy_g2", receipt.get_g2());
  EXPECT_EQ("dummy_sub", id_token.get_sub());
  EXPECT_EQ("dummy_nickname", id_token.get_nickname());
  EXPECT_EQ("dummy_provider", id_token.get_provider());
  EXPECT_EQ(1234, id_token.get_exp());
}
