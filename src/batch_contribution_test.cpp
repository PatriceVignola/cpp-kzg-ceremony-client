#include "include/batch_contribution.hpp"
#include "include/contribution_schema.hpp"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

class TestContribution : public ::testing::TestWithParam<int> {};

static constexpr int ecdsa_signature_length = 132;
static constexpr int bls_signature_length = 98;

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(TestContributionSuite, TestContribution,
                         ::testing::Values(0, 1, 2, 3));

static nlohmann::json generate_initial_contribution() {
  static constexpr int num_g2_powers = 65;
  static constexpr int num_g1_powers_contribution_1 = 4096;
  static constexpr int num_g1_powers_contribution_2 = 8192;
  static constexpr int num_g1_powers_contribution_3 = 16384;
  static constexpr int num_g1_powers_contribution_4 = 32768;
  static constexpr auto pot_pubkey =
      "0x93e02b6052719f607dacd3a088274f65596bd0d09920b61ab5da61bbdc7f5049334cf1"
      "1213945d57e5ac7d055d042b7e024aa2b2f08f0a91260805272dc51051c6e47ad4fa403b"
      "02b4510b647ae3d1770bac0326a805bbefd48056c8c121bdb8";

  static constexpr auto g1_power =
      "0x97f1d3a73197d7942695638c4fa9ac0fc3688c4f9774b905a14e3a3f171bac586c55e8"
      "3ff97a1aeffb3af00adb22c6bb";

  static constexpr auto g2_power =
      "0x93e02b6052719f607dacd3a088274f65596bd0d09920b61ab5da61bbdc7f5049334cf1"
      "1213945d57e5ac7d055d042b7e024aa2b2f08f0a91260805272dc51051c6e47ad4fa403b"
      "02b4510b647ae3d1770bac0326a805bbefd48056c8c121bdb8";

  nlohmann::json initial_contribution = {
      {
          "contributions",
          {
              {
                  {"numG1Powers", num_g1_powers_contribution_1},
                  {"numG2Powers", num_g2_powers},
                  {
                      "powersOfTau",
                      {
                          {
                              "G1Powers",
                              std::vector<std::string>(
                                  num_g1_powers_contribution_1, g1_power),
                          },
                          {
                              "G2Powers",
                              std::vector<std::string>(num_g2_powers, g2_power),
                          },
                      },
                  },
                  {"potPubkey", pot_pubkey},
              },
              {
                  {"numG1Powers", num_g1_powers_contribution_2},
                  {"numG2Powers", num_g2_powers},
                  {
                      "powersOfTau",
                      {
                          {
                              "G1Powers",
                              std::vector<std::string>(
                                  num_g1_powers_contribution_2, g1_power),
                          },
                          {
                              "G2Powers",
                              std::vector<std::string>(num_g2_powers, g2_power),
                          },
                      },
                  },
                  {"potPubkey", pot_pubkey},
              },
              {
                  {"numG1Powers", num_g1_powers_contribution_3},
                  {"numG2Powers", num_g2_powers},
                  {
                      "powersOfTau",
                      {
                          {
                              "G1Powers",
                              std::vector<std::string>(
                                  num_g1_powers_contribution_3, g1_power),
                          },
                          {
                              "G2Powers",
                              std::vector<std::string>(num_g2_powers, g2_power),
                          },
                      },
                  },
                  {"potPubkey", pot_pubkey},
              },
              {
                  {"numG1Powers", num_g1_powers_contribution_4},
                  {"numG2Powers", num_g2_powers},
                  {
                      "powersOfTau",
                      {
                          {
                              "G1Powers",
                              std::vector<std::string>(
                                  num_g1_powers_contribution_4, g1_power),
                          },
                          {
                              "G2Powers",
                              std::vector<std::string>(num_g2_powers, g2_power),
                          },
                      },
                  },
                  {"potPubkey", pot_pubkey},
              },
          },
      },
  };

  return initial_contribution;
}

// NOLINTNEXTLINE
TEST(TestBatchContribution, ParsesJsonCorrectly) {
  auto batch_contribution_json = generate_initial_contribution();

  // Add an ECDSA signature since the initial contribution doesn't have one
  std::string ecdsa_signature = "0x";
  ecdsa_signature.resize(ecdsa_signature_length, 'a');
  batch_contribution_json["ecdsaSignature"] = ecdsa_signature;

  // Add a BLS signature since the initial contribution doesn't have one
  std::string bls_signature = "0x";
  bls_signature.resize(bls_signature_length, 'b');

  for (auto& contribution_json : batch_contribution_json["contributions"]) {
    contribution_json["blsSignature"] = bls_signature;
  }

  std::cout << "1" << std::endl;
  BatchContribution batch_contribution(batch_contribution_json,
                                       contribution_schema_json);
  std::cout << "2" << std::endl;

  const auto& contributions_json = batch_contribution_json.at("contributions");
  std::cout << "3" << std::endl;
  ASSERT_EQ(contributions_json.size(),
            batch_contribution.get_contributions().size());
  std::cout << "4" << std::endl;

  for (size_t i = 0; i < batch_contribution.get_contributions().size(); ++i) {
    std::cout << i << ": 5" << std::endl;
    const auto& contribution = batch_contribution.get_contributions()[i];
    std::cout << i << ": 6" << std::endl;
    const auto& contribution_json = contributions_json.at(i);
    std::cout << i << ": 7" << std::endl;

    EXPECT_EQ(contribution_json.at("numG1Powers"),
              contribution.get_num_g1_powers());
    std::cout << i << ": 8" << std::endl;
    EXPECT_EQ(contribution_json.at("numG2Powers"),
              contribution.get_num_g2_powers());
    std::cout << i << ": 9" << std::endl;

    const auto& powers_of_tau = contribution.get_powers_of_tau();
    std::cout << i << ": 10" << std::endl;
    const auto& g1_powers = powers_of_tau.get_g1_powers();
    std::cout << i << ": 11" << std::endl;
    const auto& g2_powers = powers_of_tau.get_g2_powers();
    std::cout << i << ": 12" << std::endl;

    const auto& powers_of_tau_json = contribution_json.at("powersOfTau");
    std::cout << i << ": 13" << std::endl;
    const auto& g1_powers_json = powers_of_tau_json.at("G1Powers");
    std::cout << i << ": 14" << std::endl;
    const auto& g2_powers_json = powers_of_tau_json.at("G2Powers");
    std::cout << i << ": 15" << std::endl;

    EXPECT_EQ(g1_powers_json.size(), g1_powers.size());
    std::cout << i << ": 16" << std::endl;
    EXPECT_EQ(g2_powers_json.size(), g2_powers.size());
    std::cout << i << ": 17" << std::endl;
  }

  std::cout << "18" << std::endl;
  auto serialized_batch_contribution =
      nlohmann::json(batch_contribution).dump();
  std::cout << "19" << std::endl;
  auto serialized_batch_contribution_json = batch_contribution_json.dump();
  std::cout << "20" << std::endl;

  auto diff = nlohmann::json::diff(nlohmann::json(batch_contribution),
                                   batch_contribution_json);
  std::cout << "21" << std::endl;

  EXPECT_TRUE(diff.empty());
  std::cout << "22" << std::endl;
}

// NOLINTNEXTLINE
TEST(TestBatchContribution, ThrowsErrorIfEcdsaSignatureTooShort) {
  auto batch_contribution_json = generate_initial_contribution();

  std::string ecdsa_signature = "0x";
  ecdsa_signature.resize(ecdsa_signature_length - 1, 'a');
  batch_contribution_json["ecdsaSignature"] = ecdsa_signature;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(
              error.what(),
              ::testing::HasSubstr(
                  "Failed to match regex specified by 'pattern' constraint."));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST(TestBatchContribution, ThrowsErrorIfEcdsaSignatureTooLong) {
  auto batch_contribution_json = generate_initial_contribution();

  std::string ecdsa_signature = "0x";
  ecdsa_signature.resize(ecdsa_signature_length + 1, 'a');
  batch_contribution_json["ecdsaSignature"] = ecdsa_signature;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(
              error.what(),
              ::testing::HasSubstr(
                  "Failed to match regex specified by 'pattern' constraint."));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST(TestBatchContribution, ThrowsErrorIfEcdsaSignatureInvalidCharacter) {
  auto batch_contribution_json = generate_initial_contribution();

  std::string ecdsa_signature = "0x";
  ecdsa_signature.resize(ecdsa_signature_length, 'z');
  batch_contribution_json["ecdsaSignature"] = ecdsa_signature;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(
              error.what(),
              ::testing::HasSubstr(
                  "Failed to match regex specified by 'pattern' constraint."));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST(TestBatchContribution, ThrowsErrorIfMissingContributionsKey) {
  auto batch_contribution_json = generate_initial_contribution();
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

// NOLINTNEXTLINE
TEST(TestBatchContribution, ThrowsErrorIfLessThan4Contributions) {
  auto batch_contribution_json = generate_initial_contribution();
  batch_contribution_json.at("contributions").erase(3);

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(error.what(),
                      ::testing::HasSubstr(
                          "Array should contain no fewer than 4 elements."));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfNumG1PowersTooHigh) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);
  auto num_g1_powers = first_contribution.at("numG1Powers").get<int>();
  first_contribution["numG1Powers"] = num_g1_powers + 1;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(
              error.what(),
              ::testing::HasSubstr("Expected number less than or equal to " +
                                   std::to_string(num_g1_powers)));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfNumG1PowersTooLow) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);
  auto num_g1_powers = first_contribution.at("numG1Powers").get<int>();
  first_contribution["numG1Powers"] = num_g1_powers - 1;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(
              error.what(),
              ::testing::HasSubstr("Expected number greater than or equal to " +
                                   std::to_string(num_g1_powers)));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfNumG2PowersMissing) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);
  first_contribution.erase("numG2Powers");

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(
              error.what(),
              ::testing::HasSubstr("Missing required property 'numG2Powers'."));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfNumG2PowersMoreThan65) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);
  static constexpr int bad_number = 66;
  first_contribution["numG2Powers"] = bad_number;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(
              error.what(),
              ::testing::HasSubstr("Expected number less than or equal to 65"));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfNumG2PowersLessThan65) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);
  static constexpr int bad_number = 64;
  first_contribution["numG2Powers"] = bad_number;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(error.what(),
                      ::testing::HasSubstr(
                          "Expected number greater than or equal to 65"));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfPowersOfTauMissing) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);
  first_contribution.erase("powersOfTau");

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(
              error.what(),
              ::testing::HasSubstr("Missing required property 'powersOfTau'."));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfPowersOfTauNotObject) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);
  first_contribution["powersOfTau"] = "foo";

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(error.what(),
                      ::testing::HasSubstr(
                          "Value type not permitted by 'type' constraint."));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfNotEnoughG1Powers) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);
  auto& powers_of_tau = first_contribution.at("powersOfTau");
  auto& g1_powers = powers_of_tau.at("G1Powers");
  auto expected_num_elements = g1_powers.size();
  g1_powers.erase(0);

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(error.what(), ::testing::HasSubstr(
                                        "Array should contain no fewer than " +
                                        std::to_string(expected_num_elements) +
                                        " elements."));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfTooManyG1Powers) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);
  auto& powers_of_tau = first_contribution.at("powersOfTau");
  auto& g1_powers = powers_of_tau.at("G1Powers");
  auto expected_num_elements = g1_powers.size();
  g1_powers.emplace_back(g1_powers.at(0));

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(error.what(), ::testing::HasSubstr(
                                        "Array should contain no more than " +
                                        std::to_string(expected_num_elements) +
                                        " elements."));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfG1PowerHexTooShort) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);
  auto& powers_of_tau = first_contribution.at("powersOfTau");
  auto& g1_powers = powers_of_tau.at("G1Powers");
  auto g1_power = g1_powers.at(0).get<std::string>();
  g1_power.pop_back();
  g1_powers[0] = g1_power;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(
              error.what(),
              ::testing::HasSubstr(
                  "Failed to match regex specified by 'pattern' constraint."));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfG1PowerHexTooLong) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);
  auto& powers_of_tau = first_contribution.at("powersOfTau");
  auto& g1_powers = powers_of_tau.at("G1Powers");
  auto g1_power = g1_powers.at(0).get<std::string>();
  g1_power.push_back('a');
  g1_powers[0] = g1_power;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(
              error.what(),
              ::testing::HasSubstr(
                  "Failed to match regex specified by 'pattern' constraint."));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfG1PowerHexInvalidCharacter) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);
  auto& powers_of_tau = first_contribution.at("powersOfTau");
  auto& g1_powers = powers_of_tau.at("G1Powers");
  auto g1_power = g1_powers.at(0).get<std::string>();
  g1_power.back() = 'z';
  g1_powers[0] = g1_power;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(
              error.what(),
              ::testing::HasSubstr(
                  "Failed to match regex specified by 'pattern' constraint."));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfLessThan65G2Powers) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);
  auto& powers_of_tau = first_contribution.at("powersOfTau");
  powers_of_tau.at("G2Powers").erase(0);

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(error.what(),
                      ::testing::HasSubstr(
                          "Array should contain no fewer than 65 elements."));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfMoreThan65G2Powers) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);
  auto& powers_of_tau = first_contribution.at("powersOfTau");
  auto& g2_powers = powers_of_tau.at("G2Powers");
  g2_powers.emplace_back(g2_powers.at(0));

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(error.what(),
                      ::testing::HasSubstr(
                          "Array should contain no more than 65 elements."));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfG2PowerHexTooShort) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);
  auto& powers_of_tau = first_contribution.at("powersOfTau");
  auto& g2_powers = powers_of_tau.at("G2Powers");
  auto g2_power = g2_powers.at(0).get<std::string>();
  g2_power.pop_back();
  g2_powers[0] = g2_power;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(
              error.what(),
              ::testing::HasSubstr(
                  "Failed to match regex specified by 'pattern' constraint."));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfG2PowerHexTooLong) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);
  auto& powers_of_tau = first_contribution.at("powersOfTau");
  auto& g2_powers = powers_of_tau.at("G2Powers");
  auto g2_power = g2_powers.at(0).get<std::string>();
  g2_power.push_back('a');
  g2_powers[0] = g2_power;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(
              error.what(),
              ::testing::HasSubstr(
                  "Failed to match regex specified by 'pattern' constraint."));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfG2PowerHexInvalidCharacter) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);
  auto& powers_of_tau = first_contribution.at("powersOfTau");
  auto& g2_powers = powers_of_tau.at("G2Powers");
  auto g2_power = g2_powers.at(0).get<std::string>();
  g2_power.back() = 'z';
  g2_powers[0] = g2_power;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(
              error.what(),
              ::testing::HasSubstr(
                  "Failed to match regex specified by 'pattern' constraint."));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfPotPubkeyMissing) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);
  first_contribution.erase("potPubkey");

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (
            const nlohmann::json_abi_v3_11_2::detail::out_of_range& error) {
          EXPECT_STREQ(
              "[json.exception.out_of_range.403] key 'potPubkey' not found",
              error.what());
          throw;
        }
      },
      nlohmann::json_abi_v3_11_2::detail::out_of_range);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfPotPubkeyNotString) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);
  static constexpr int bad_pot_pubkey_value = 1234;
  first_contribution.at("potPubkey") = bad_pot_pubkey_value;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(error.what(),
                      ::testing::HasSubstr(
                          "Value type not permitted by 'type' constraint."));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfBlsSignatureTooShort) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);

  std::string bls_signature = "0x";
  bls_signature.resize(bls_signature_length - 1, 'a');
  batch_contribution_json["blsSignature"] = bls_signature;
  first_contribution["blsSignature"] = bls_signature;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(
              error.what(),
              ::testing::HasSubstr(
                  "Failed to match regex specified by 'pattern' constraint."));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfBlsSignatureTooLong) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);

  std::string bls_signature = "0x";
  bls_signature.resize(bls_signature_length + 1, 'a');
  batch_contribution_json["blsSignature"] = bls_signature;
  first_contribution["blsSignature"] = bls_signature;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(
              error.what(),
              ::testing::HasSubstr(
                  "Failed to match regex specified by 'pattern' constraint."));
          throw;
        }
      },
      std::runtime_error);
}

// NOLINTNEXTLINE
TEST_P(TestContribution, ThrowsErrorIfBlsSignatureInvalidCharacter) {
  auto batch_contribution_json = generate_initial_contribution();
  auto& contributions = batch_contribution_json.at("contributions");
  const int contribution_index = GetParam();
  auto& first_contribution = contributions.at(contribution_index);

  std::string bls_signature = "0x";
  bls_signature.resize(bls_signature_length, 'z');
  batch_contribution_json["blsSignature"] = bls_signature;
  first_contribution["blsSignature"] = bls_signature;

  // NOLINTNEXTLINE
  EXPECT_THROW(
      {
        try {
          BatchContribution(batch_contribution_json, contribution_schema_json);
        } catch (const std::runtime_error& error) {
          EXPECT_THAT(
              error.what(),
              ::testing::HasSubstr(
                  "Failed to match regex specified by 'pattern' constraint."));
          throw;
        }
      },
      std::runtime_error);
}
