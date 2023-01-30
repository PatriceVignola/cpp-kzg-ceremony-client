#include "include/bls_signer.hpp"
#include <string>
#include <vector>

#ifdef _DLL
#undef _DLL
#include <uint256_t.h>
#define _DLL
#else
#include <uint256_t.h>
#endif

namespace bls_signer {
blst::P1_Affine sign(uint256_t secret, const std::string& identity) {
  blst::P1 point;
  std::vector<uint8_t> identity_bytes(identity.begin(), identity.end());

  blst::SecretKey secret_key{};
  secret_key.from_bendian(static_cast<uint8_t*>(static_cast<void*>(&secret)));

  point
      .hash_to(identity_bytes.data(), identity_bytes.size(),
               "BLS_SIG_BLS12381G1_XMD:SHA-256_SSWU_RO_POP_")
      ->sign_with(secret_key);

  return point.to_affine();
}
} // namespace bls_signer