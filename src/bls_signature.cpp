#include "include/bls_signature.hpp"
#include "include/bls_signer.hpp"
#include "include/hex_util.hpp"
#include <absl/strings/string_view.h>
#include <array>
#include <blst.hpp>
#include <string>

BlsSignature::BlsSignature(blst::Scalar secret, absl::string_view identity)
    : bls_signature_(bls_signer::sign(secret, identity)) {}

std::string BlsSignature::encode() const {
  static constexpr size_t compressed_size_in_bytes = 48;
  std::array<uint8_t, 48> bls_signature_bytes{};
  bls_signature_.compress(bls_signature_bytes.data());

  return hex_util::encode(bls_signature_bytes);
}