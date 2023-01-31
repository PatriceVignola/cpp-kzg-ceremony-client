#include "include/contribution_receipt.hpp"
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

// NOLINTNEXTLINE
TEST(TestContributionReceipt, ThrowsErrorIfReceiptNotFound) {
  static const auto contribution_receipt_json = R"(
    {
      "signature": "foo"
    }
  )"_json;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          contribution_receipt_json.get<ContributionReceipt>();
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
  static const auto contribution_receipt_json = R"(
    {
      "receipt": "{\"identity\": \"git|12345|foo\", \"witness\": [\"0x123\", \"0x456\"]}"
    }
  )"_json;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          contribution_receipt_json.get<ContributionReceipt>();
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
  static const auto contribution_receipt_json = R"(
    {
      "signature": "dummy_signature",
      "receipt": "{\"identity\": \"git|12345|foo\", \"witness\": [\"0x123\", \"0x456\"]}"
    }
  )"_json;

  const auto contribution_receipt =
      contribution_receipt_json.get<ContributionReceipt>();

  EXPECT_EQ("dummy_signature", contribution_receipt.get_signature());
  EXPECT_EQ("git|12345|foo", contribution_receipt.get_receipt().get_identity());
  EXPECT_EQ(std::vector<std::string>({"0x123", "0x456"}),
            contribution_receipt.get_receipt().get_witness());
}
