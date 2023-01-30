#include "include/contribution.hpp"
#include <blst.hpp>

#ifdef _DLL
#undef _DLL
#include <uint256_t.h>
#define _DLL
#else
#include <uint256_t.h>
#endif

void from_json(const nlohmann::json& json_contribution,
               Contribution& contribution) {
  json_contribution.at("numG1Powers").get_to(contribution.num_g1_powers_);
  json_contribution.at("numG2Powers").get_to(contribution.num_g2_powers_);
  json_contribution.at("powersOfTau").get_to(contribution.powers_of_tau_);
  json_contribution.at("potPubkey").get_to(contribution.pot_pubkey_);

  if (json_contribution.find("bls_signature") != json_contribution.end()) {
    json_contribution.at("bls_signature").get_to(contribution.bls_signature_);
  }
}

void to_json(nlohmann::json& json_contribution,
             const Contribution& contribution) {
  json_contribution["numG1Powers"] = contribution.num_g1_powers_;
  json_contribution["numG2Powers"] = contribution.num_g2_powers_;
  json_contribution["powersOfTau"] = contribution.powers_of_tau_;
  json_contribution["potPubkey"] = contribution.pot_pubkey_;
  json_contribution["bls_signature"] = contribution.bls_signature_;
}

void Contribution::update_powers_of_tau(uint256_t secret) {
  uint256_t current_power = 1;
  auto& g1_powers = powers_of_tau_.get_g1_powers();
  auto& g2_powers = powers_of_tau_.get_g2_powers();

  static constexpr size_t num_scalar_bytes = 32;
  static constexpr size_t scalar_bits = 256;
  std::vector<uint8_t> scalar_bytes(num_scalar_bytes);
  scalar_bytes[0] = 1;
  auto scalar = blst::Scalar(scalar_bytes.data(), scalar_bits);

  blst::Scalar secret_scalar(
      static_cast<const uint8_t*>(static_cast<const void*>(&secret)),
      scalar_bits);

  for (size_t i = 0; i < g1_powers.size(); ++i) {
    g1_powers[i].multiply(scalar);

    if (i < g2_powers.size()) {
      g2_powers[i].multiply(scalar);
    }

    scalar.mul(secret_scalar);
  }
}