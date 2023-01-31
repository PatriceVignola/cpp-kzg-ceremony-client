#include "include/bls_signer.hpp"
#include <array>
#include <string>
#include <vector>

namespace bls_signer {
blst::P1_Affine sign(blst::Scalar secret, const std::string& identity) {
  blst::P1 point;
  std::vector<uint8_t> identity_bytes(identity.begin(), identity.end());

  std::array<uint8_t, sizeof(secret)> secret_bytes{};
  secret.to_bendian(secret_bytes.data());

  blst::SecretKey secret_key{};
  secret_key.from_bendian(secret_bytes.data());

  point
      .hash_to(identity_bytes.data(), identity_bytes.size(),
               "BLS_SIG_BLS12381G1_XMD:SHA-256_SSWU_RO_POP_")
      ->sign_with(secret_key);

  return point.to_affine();
}
} // namespace bls_signer