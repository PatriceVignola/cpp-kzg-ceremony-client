#include "include/batch_contribution.hpp"
#include "include/contribution_schema.hpp"
#include "include/test_data/initial_contribution.hpp"
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

// NOLINTNEXTLINE
TEST(TestBatchContribution, ThrowsErrorIfMissingContributionsKey) {
  auto batch_contribution_json = initial_contribution_json;
  batch_contribution_json.erase("contributions");

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (
            const nlohmann::json_abi_v3_11_2::detail::out_of_range& error) {
          EXPECT_STREQ(
              "[json.exception.out_of_range.403] key 'contributions' not found",
              error.what());
          throw;
        }
      },
      nlohmann::json_abi_v3_11_2::detail::out_of_range);
}