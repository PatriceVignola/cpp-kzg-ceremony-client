#include "include/secret_generator.hpp"
#include <blst.hpp>
#include <duthomhas/csprng.hpp>

SecretGenerator::SecretGenerator(const std::string& entropy_string,
                                 size_t num_secrets) {
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
    duthomhas::csprng rng;

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    rng(entropy_bytes.data() + random_bytes_start, num_random_bytes);

    // Finally, feed the bytes into the BLS keygen
    auto secret_key = blst::SecretKey();
    secret_key.keygen_v3(entropy_bytes.data(), entropy_bytes.size());

    uint256_t secret = 0;
    secret_key.to_bendian(static_cast<uint8_t*>(static_cast<void*>(&secret)));
    secrets_.insert(secret);
  }
}
