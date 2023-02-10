#include "include/hex_util.hpp"
#include "include/identity_fetcher.hpp"
#include <gtest/gtest.h>

// NOLINTNEXTLINE
TEST(IdentityFetcher, CorrectlyParsesEthereumAddress) {
  const std::string input_address =
      "0xAe584B9034D150CDb4934Ba7fE46798424FFC64F";
  const auto identity = identity_fetcher::get_ethereum_identity(input_address);
  const std::string expected_identity =
      "eth|0xae584b9034d150cdb4934ba7fe46798424ffc64f";

  EXPECT_EQ(expected_identity, identity);
}

// NOLINTNEXTLINE
TEST(IdentityFetcher, ThrowsErrorIfEthereumAddressNo0x) {
  const std::string input_address = "Ae584B9034D150CDb4934Ba7fE46798424FFC64F";

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          identity_fetcher::get_ethereum_identity(input_address);
        } catch (const std::runtime_error& error) {
          EXPECT_STREQ(
              "Ethereum address `Ae584B9034D150CDb4934Ba7fE46798424FFC64F` "
              "doesn't start with `0x`",
              error.what());
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST(IdentityFetcher, ThrowsErrorIfEthereumAddressTooLong) {
  const std::string input_address =
      "0xAe584B9034D150CDb4934Ba7fE46798424FFC64FF";

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          identity_fetcher::get_ethereum_identity(input_address);
        } catch (const std::runtime_error& error) {
          EXPECT_STREQ(
              "Ethereum address `0xAe584B9034D150CDb4934Ba7fE46798424FFC64FF` "
              "doesn't have 42 characters",
              error.what());
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST(IdentityFetcher, ThrowsErrorIfEthereumAddressTooShort) {
  const std::string input_address = "0xAe584B9034D150CDb4934Ba7fE46798424FFC64";

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          identity_fetcher::get_ethereum_identity(input_address);
        } catch (const std::runtime_error& error) {
          EXPECT_STREQ(
              "Ethereum address `0xAe584B9034D150CDb4934Ba7fE46798424FFC64` "
              "doesn't have 42 characters",
              error.what());
          throw;
        }
      },
      std::runtime_error);
}
