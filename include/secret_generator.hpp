#ifndef SECRET_GENERATOR_HPP
#define SECRET_GENERATOR_HPP

#include <array>
#include <blst.hpp>
#include <cstdint>
#include <duthomhas/csprng.hpp>
#include <set>
#include <string>

template <typename csprng = duthomhas::csprng,
          typename BlstSecretKey = blst::SecretKey>
class SecretGenerator {
public:
  SecretGenerator(const std::vector<uint8_t>& entropy, size_t num_secrets,
                  csprng&& generator = duthomhas::csprng(),
                  BlstSecretKey&& blst_secret_key = BlstSecretKey()) {
    while (secrets_.size() < num_secrets) {
      static constexpr size_t min_entropy_bytes = 256;
      std::vector<uint8_t> entropy_bytes = entropy;

      // Replace at least half the initial+padded bytes with random ones
      const size_t num_random_bytes = std::max(
          min_entropy_bytes - entropy_bytes.size(), min_entropy_bytes / 2);

      // If the input has less than 256 bytes, we add some padding that will be
      // randomized below
      if (entropy_bytes.size() < min_entropy_bytes) {
        entropy_bytes.resize(min_entropy_bytes);
      }

      const size_t random_bytes_start = entropy_bytes.size() - num_random_bytes;

      // Randomize the end of the entropy bytes with a CSPRNG
      // - On Windows, CryptGenRandom is used
      // - Otherwise, /dev/urandom is used
      // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
      generator(entropy_bytes.data() + random_bytes_start, num_random_bytes);

      // Finally, feed the bytes into the BLS keygen
      blst_secret_key.keygen_v3(entropy_bytes.data(), entropy_bytes.size());

      std::array<uint8_t, sizeof(blst::Scalar)> secret_key_bytes{};
      blst_secret_key.to_bendian(secret_key_bytes.data());

      for (const auto& other_secret : secrets_) {
        std::array<uint8_t, sizeof(blst::Scalar)> other_bytes{};
        other_secret.to_bendian(other_bytes.data());

        if (std::equal(secret_key_bytes.begin(), secret_key_bytes.end(),
                       other_bytes.begin())) {
          continue;
        }
      }

      blst::Scalar secret;
      secret.from_bendian(secret_key_bytes.data(), secret_key_bytes.size());
      secrets_.push_back(secret);
    }
  }

  const std::vector<blst::Scalar>& get_secrets() const { return secrets_; }

private:
  // Since we only need to generate 4 secrets and and collisions are expected to
  // be rare, using a vector instead of a set makes more sense here
  std::vector<blst::Scalar> secrets_;
};

#endif // SECRET_GENERATOR_HPP