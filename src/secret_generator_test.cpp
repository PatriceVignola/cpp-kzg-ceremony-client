#include "include/secret_generator.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <numeric>

class MockCsprng {
public:
  // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
  MOCK_METHOD2(parentheses_op, void(uint8_t* buffer, size_t n));

  // NOLINTNEXTLINE(fuchsia-overloaded-operator)
  void operator()(uint8_t* buffer, size_t n) { parentheses_op(buffer, n); }
};

class MockBlstSecretKey {
public:
  // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
  MOCK_METHOD2(keygen_v3, void(const uint8_t* buffer, size_t n));

  // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
  MOCK_METHOD1(to_bendian, void(uint8_t* secret));
};

// NOLINTNEXTLINE
TEST(TestSecretGenerator, GeneratesNothingWhenZeroSecrets) {
  MockCsprng csprng;
  EXPECT_CALL(csprng, parentheses_op(::testing::_, ::testing::_)).Times(0);

  SecretGenerator<MockCsprng> secret_generator({}, 0, std::move(csprng));
  EXPECT_EQ(0, secret_generator.get_secrets().size());
}

// NOLINTNEXTLINE
TEST(TestSecretGenerator, Generates256RandomBytesWhenEmptyEntropy) {
  MockCsprng csprng;
  MockBlstSecretKey blst_secret_key;

  static constexpr size_t min_csprng_values = 256;
  std::vector<uint8_t> expected_csprng_values(min_csprng_values);
  std::iota(expected_csprng_values.begin(), expected_csprng_values.end(), 0);

  EXPECT_CALL(csprng, parentheses_op(::testing::_, min_csprng_values))
      .WillOnce(::testing::SetArrayArgument<0>(expected_csprng_values.begin(),
                                               expected_csprng_values.end()));

  EXPECT_CALL(blst_secret_key, keygen_v3(::testing::_, min_csprng_values))
      .With(::testing::Args<0, 1>(::testing::ElementsAreArray(
          expected_csprng_values.data(), min_csprng_values)))
      .Times(1);

  static constexpr size_t num_secret_bytes = 32;
  std::vector<uint8_t> expected_secret_bytes(num_secret_bytes);
  std::iota(expected_secret_bytes.begin(), expected_secret_bytes.end(), 0);

  EXPECT_CALL(blst_secret_key, to_bendian(::testing::_))
      .WillOnce(::testing::SetArrayArgument<0>(expected_secret_bytes.begin(),
                                               expected_secret_bytes.end()));

  blst::Scalar secret_key;
  secret_key.from_bendian(expected_secret_bytes.data(),
                          expected_secret_bytes.size());

  SecretGenerator<MockCsprng, MockBlstSecretKey> secret_generator(
      {}, 1, std::move(csprng), std::move(blst_secret_key));

  const auto& secrets = secret_generator.get_secrets();
  EXPECT_EQ(1, secret_generator.get_secrets().size());

  EXPECT_TRUE(
      std::any_of(secrets.begin(), secrets.end(),
                  [&expected_secret_bytes](const blst::Scalar& secret) {
                    std::array<uint8_t, sizeof(secret_key)> secret_bytes{};
                    secret.to_bendian(secret_bytes.data());

                    return std::equal(secret_bytes.begin(), secret_bytes.end(),
                                      expected_secret_bytes.begin());
                  }));
}
