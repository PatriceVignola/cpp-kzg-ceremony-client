#include "include/receipt.hpp"
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

// NOLINTNEXTLINE
TEST(TestReceipt, ThrowsErrorIfWitnessNotFound) {
  static const auto receipt_json = R"(
    {
      "identity": "git|12345|foo"
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
              "[json.exception.out_of_range.403] key 'witness' not found",
              error.what());
          throw;
        }
      },
      nlohmann::json_abi_v3_11_2::detail::out_of_range);
}

// NOLINTNEXTLINE
TEST(TestReceipt, ThrowsErrorIfIdentityNotFound) {
  static const auto receipt_json = R"(
    {
      "witness": [
        "0x123",
        "0x456"
      ]
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
              "[json.exception.out_of_range.403] key 'identity' not found",
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
      "identity": "git|12345|foo",
      "witness": [
        "0x123",
        "0x456"
      ]
    }
  )"_json;

  const auto receipt = receipt_json.get<Receipt>();
  const auto& identity = receipt.get_identity();
  const auto& witness = receipt.get_witness();

  EXPECT_EQ("git|12345|foo", identity);
  EXPECT_EQ(std::vector<std::string>({"0x123", "0x456"}), witness);
}
