#include "include/g1_power.hpp"
#include "include/uint256_helpers.hpp"

void G1Power::multiply(uint256_t power) {
  static constexpr size_t num_bits = 256;
  const auto* power_bytes =
      static_cast<const uint8_t*>(static_cast<void*>(&power));
  p1_.mult(power_bytes, num_bits);
}