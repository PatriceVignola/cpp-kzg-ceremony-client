#ifndef G2_POWER_H
#define G2_POWER_H

#include "hex_util.h"
#include <blst.hpp>

class G2Power {
public:
  explicit G2Power(const std::string& hex_power)
      : p2_(hex_util::decode(hex_power).data()) {}
  bool in_group() const { return p2_.in_group(); }

private:
  blst::P2 p2_;
};

#endif // G2_POWER_H