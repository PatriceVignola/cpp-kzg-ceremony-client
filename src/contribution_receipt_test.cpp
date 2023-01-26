#include "include/contribution_receipt.hpp"
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

// NOLINTNEXTLINE
TEST(TestContributionReceipt, ThrowsErrorIfReceiptNotFound) {
  static const auto contribution_error_json = R"(
    {
      "signature": "foo"
    }
  )"_json;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          contribution_error_json.get<ContributionReceipt>();
        } catch (
            const nlohmann::json_abi_v3_11_2::detail::out_of_range& error) {
          EXPECT_STREQ(
              "[json.exception.out_of_range.403] key 'receipt' not found",
              error.what());
          throw;
        }
      },
      nlohmann::json_abi_v3_11_2::detail::out_of_range);
}

// NOLINTNEXTLINE
TEST(TestContributionReceipt, ThrowsErrorIfSignatureNotFound) {
  static const auto contribution_error_json = R"(
    {
      "receipt": "{\"id_token\": {\"sub\": \"dummy_sub\",\"nickname\": \"dummy_nickname\",\"provider\": \"dummy_provider\",\"exp\": 1234},\"g2\": \"dummy_g2\"}"
    }
  )"_json;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          contribution_error_json.get<ContributionReceipt>();
        } catch (
            const nlohmann::json_abi_v3_11_2::detail::out_of_range& error) {
          EXPECT_STREQ(
              "[json.exception.out_of_range.403] key 'signature' not found",
              error.what());
          throw;
        }
      },
      nlohmann::json_abi_v3_11_2::detail::out_of_range);
}

// NOLINTNEXTLINE
TEST(TestContributionReceipt, ParsesJsonCorrectly) {
  static const auto contribution_error_json = R"(
    {
      "signature": "dummy_signature",
      "receipt": "{\"id_token\": {\"sub\": \"dummy_sub\",\"nickname\": \"dummy_nickname\",\"provider\": \"dummy_provider\",\"exp\": 1234},\"g2\": \"dummy_g2\"}"
    }
  )"_json;

  const auto contribution_receipt =
      contribution_error_json.get<ContributionReceipt>();

  EXPECT_EQ("dummy_signature", contribution_receipt.get_signature());
  EXPECT_EQ("dummy_sub",
            contribution_receipt.get_receipt().get_id_token().get_sub());
  EXPECT_EQ("dummy_nickname",
            contribution_receipt.get_receipt().get_id_token().get_nickname());
  EXPECT_EQ("dummy_provider",
            contribution_receipt.get_receipt().get_id_token().get_provider());
  EXPECT_EQ(1234, contribution_receipt.get_receipt().get_id_token().get_exp());
  EXPECT_EQ("dummy_g2", contribution_receipt.get_receipt().get_g2());
}
