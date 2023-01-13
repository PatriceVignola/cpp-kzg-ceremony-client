#ifndef G1_POWER_H
#define G1_POWER_H

#include "hex_util.h"
#include <blst.hpp>

class G1Power {
public:
  explicit G1Power(const std::string& hex_power)
      : p1_(hex_util::decode(hex_power).data()) {}
  bool in_group() const { return p1_.in_group(); }

private:
  blst::P1 p1_;
};

#endif // G1_POWER_H