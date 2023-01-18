#ifndef G1_POWER_HPP
#define G1_POWER_HPP

#include "hex_util.hpp"
#include <blst.hpp>
#include <uint256_t.h>

class G1Power {
public:
  explicit G1Power(const std::string& hex_power)
      : p1_(hex_util::decode(hex_power).data()) {}
  bool in_group() const { return p1_.in_group(); }
  void multiply(uint256_t power);

private:
  blst::P1 p1_;
};

#endif // G1_POWER_HPP