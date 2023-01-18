#include "include/contribution.hpp"
#include "include/uint256_helpers.hpp"

static constexpr uint8_t curve_order_base = 10;

void Contribution::update_powers_of_tau(uint256_t secret) {
  uint256_t current_power = 1;
  auto& g1_powers = powers_of_tau_.get_g1_powers();
  auto& g2_powers = powers_of_tau_.get_g2_powers();

  for (size_t i = 0; i < g1_powers.size(); ++i) {
    g1_powers[i].multiply(current_power);

    if (i < g2_powers.size()) {
      g2_powers[i].multiply(current_power);
    }

    // clang-format off
    static const uint256_t curve_order("52435875175126190479447740508185965837690552500527637822603658699938581184513",
                                       curve_order_base);
    // clang-format on

    current_power = (current_power * secret) % curve_order;
  }
}