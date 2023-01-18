#include "include/powers_of_tau.hpp"
#include <algorithm>

std::vector<G1Power>& PowersOfTau::get_g1_powers() { return g1_powers; }

std::vector<G2Power>& PowersOfTau::get_g2_powers() { return g2_powers; }

bool PowersOfTau::valid() const {
  return std::all_of(g1_powers.begin(), g1_powers.end(),
                     [](const auto& power) { return power.in_group(); }) &&
         std::all_of(g2_powers.begin(), g2_powers.end(),
                     [](const auto& power) { return power.in_group(); });
}
