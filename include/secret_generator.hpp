#ifndef SECRET_GENERATOR_HPP
#define SECRET_GENERATOR_HPP

#include <array>
#include <blst.hpp>
#include <cstdint>
#include <duthomhas/csprng.hpp>
#include <set>
#include <string>

#ifdef _DLL
#undef _DLL
#include <uint256_t.h>
#define _DLL
#else
#include <uint256_t.h>
#endif

template <typename csprng = duthomhas::csprng,
          typename BlstSecretKey = blst::SecretKey>
class SecretGenerator {
public:
  SecretGenerator(const std::string& entropy_string, size_t num_secrets,
                  csprng&& generator = duthomhas::csprng(),
                  BlstSecretKey&& blst_secret_key = BlstSecretKey()) {
    while (secrets_.size() < num_secrets) {
      static constexpr size_t min_entropy_bytes = 256;
      std::vector<uint8_t> entropy_bytes(entropy_string.begin(),
                                         entropy_string.end());

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

      uint256_t secret = 0;
      blst_secret_key.to_bendian(
          static_cast<uint8_t*>(static_cast<void*>(&secret)));
      secrets_.insert(secret);
    }
  }

  const std::set<uint256_t>& get_secrets() const { return secrets_; }

private:
  // Since the number of secrets is expected to be low, use std::set instead of
  // std::unordered_set
  std::set<uint256_t> secrets_;
};

#endif // SECRET_GENERATOR_HPP