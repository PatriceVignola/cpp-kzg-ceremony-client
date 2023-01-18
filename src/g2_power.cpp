#include "include/g2_power.hpp"
#ifdef _DLL
#undef _DLL
#include <uint256_t.h>
#define _DLL
#else
#include <uint256_t.h>
#endif

void G2Power::multiply(uint256_t power) {
  static constexpr size_t num_bits = 256;
  const auto* power_bytes =
      static_cast<const uint8_t*>(static_cast<void*>(&power));
  p2_.mult(power_bytes, num_bits);
}