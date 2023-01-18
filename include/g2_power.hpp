#ifndef G2_POWER_HPP
#define G2_POWER_HPP

#include "hex_util.hpp"
#include <blst.hpp>
#ifdef _DLL
#undef _DLL
#include <uint256_t.h>
#define _DLL
#else
#include <uint256_t.h>
#endif

class G2Power {
public:
  explicit G2Power(const std::string& hex_power)
      : p2_(hex_util::decode(hex_power).data()) {}
  bool in_group() const { return p2_.in_group(); }
  void multiply(uint256_t power);

private:
  blst::P2 p2_;
};

#endif // G2_POWER_HPP