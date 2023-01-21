#include "include/contribution.hpp"
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
  json_contribution.at("blsSignature").get_to(contribution.bls_signature_);
}

void to_json(nlohmann::json& json_contribution,
             const Contribution& contribution) {
  json_contribution["numG1Powers"] = contribution.num_g1_powers_;
  json_contribution["numG2Powers"] = contribution.num_g2_powers_;
  json_contribution["powersOfTau"] = contribution.powers_of_tau_;
  json_contribution["potPubkey"] = contribution.pot_pubkey_;
  json_contribution["blsSignature"] = contribution.bls_signature_;
}

void Contribution::update_powers_of_tau(uint256_t secret) {
  uint256_t current_power = 1;
  auto& g1_powers = powers_of_tau_.get_g1_powers();
  auto& g2_powers = powers_of_tau_.get_g2_powers();

  for (size_t i = 0; i < g1_powers.size(); ++i) {
    g1_powers[i].multiply(current_power);

    if (i < g2_powers.size()) {
      g2_powers[i].multiply(current_power);
    }

    static constexpr uint8_t curve_order_base = 10;

    // clang-format off
    static const uint256_t curve_order("52435875175126190479447740508185965837690552500527637822603658699938581184513",
                                       curve_order_base);
    // clang-format on

    current_power = (current_power * secret) % curve_order;
  }
}