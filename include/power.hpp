#ifndef G1_POWER_HPP
#define G1_POWER_HPP

#include <blst.hpp>
#include <nlohmann/json.hpp>

#ifdef _DLL
#undef _DLL
#include <uint256_t.h>
#define _DLL
#else
#include <uint256_t.h>
#endif

template <typename TBlstPoint, size_t size_in_bytes>
class Power {
public:
  bool in_group() const;
  void multiply(uint256_t power);

private:
  TBlstPoint blst_point_;

  template <typename TFriendBlstPoint, size_t friend_size_in_bytes>
  friend void
  from_json(const nlohmann::json& json_power_of_tau,
            Power<TFriendBlstPoint, friend_size_in_bytes>& power_of_tau);

  template <typename TFriendBlstPoint, size_t friend_size_in_bytes>
  friend void
  to_json(nlohmann::json& json_power_of_tau,
          const Power<TFriendBlstPoint, friend_size_in_bytes>& power_of_tau);
};

static constexpr size_t p1_power_size_in_bytes = 96;
using G1Power = Power<blst::P1, p1_power_size_in_bytes>;

static constexpr size_t p2_power_size_in_bytes = 192;
using G2Power = Power<blst::P2, p2_power_size_in_bytes>;

#endif // G1_POWER_HPP