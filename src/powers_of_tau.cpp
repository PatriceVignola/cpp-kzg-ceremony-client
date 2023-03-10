#include "include/powers_of_tau.hpp"
#include <absl/types/span.h>
#include <algorithm>

void from_json(const nlohmann::json& json_powers_of_tau,
               PowersOfTau& powers_of_tau) {
  json_powers_of_tau.at("G1Powers").get_to(powers_of_tau.g1_powers_);
  json_powers_of_tau.at("G2Powers").get_to(powers_of_tau.g2_powers_);
}

void to_json(nlohmann::json& json_powers_of_tau,
             const PowersOfTau& powers_of_tau) {
  json_powers_of_tau["G1Powers"] = powers_of_tau.g1_powers_;
  json_powers_of_tau["G2Powers"] = powers_of_tau.g2_powers_;
}

absl::Span<G1Power> PowersOfTau::get_g1_powers() {
  return absl::Span<G1Power>(g1_powers_);
}

absl::Span<G2Power> PowersOfTau::get_g2_powers() {
  return absl::Span<G2Power>(g2_powers_);
}

absl::Span<const G1Power> PowersOfTau::get_g1_powers() const {
  return g1_powers_;
}

absl::Span<const G2Power> PowersOfTau::get_g2_powers() const {
  return g2_powers_;
}

bool PowersOfTau::valid() const {
  return std::all_of(g1_powers_.begin(), g1_powers_.end(),
                     [](const auto& power) { return power.in_group(); }) &&
         std::all_of(g2_powers_.begin(), g2_powers_.end(),
                     [](const auto& power) { return power.in_group(); });
}
