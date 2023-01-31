#include "include/bls_signature.hpp"
#include "include/bls_signer.hpp"
#include "include/hex_util.hpp"
#include <blst.hpp>
#include <string>

BlsSignature::BlsSignature(blst::Scalar secret, const std::string& identity)
    : bls_signature_(bls_signer::sign(secret, identity)) {}

std::string BlsSignature::encode() const {
  static constexpr size_t compressed_size_in_bytes = 48;
  std::vector<uint8_t> bls_signature_bytes(compressed_size_in_bytes);
  bls_signature_.compress(bls_signature_bytes.data());

  return hex_util::encode(bls_signature_bytes);
}