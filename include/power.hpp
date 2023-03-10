#ifndef POWER_HPP
#define POWER_HPP

#include <blst.hpp>
#include <nlohmann/json.hpp>

template <typename TBlstPoint, size_t size_in_bytes>
class Power {
public:
  bool in_group() const;
  void multiply(blst::Scalar scalar);
  std::string encode() const;

  static Power<TBlstPoint, size_in_bytes> generate_pot_pubkey(
      blst::Scalar secret) {
    static constexpr size_t num_secret_bits = 256;
    Power<TBlstPoint, size_in_bytes> power;
    power.blst_point_ = TBlstPoint::generator();
    power.blst_point_.mult(secret);
    return power;
  }

private:
  TBlstPoint blst_point_;

  template <typename TFriendBlstPoint, size_t friend_size_in_bytes>
  // NOLINTNEXTLINE(readability-redundant-declaration)
  friend void from_json(
      const nlohmann::json& json_power_of_tau,
      Power<TFriendBlstPoint, friend_size_in_bytes>& power_of_tau);

  template <typename TFriendBlstPoint, size_t friend_size_in_bytes>
  // NOLINTNEXTLINE(readability-redundant-declaration)
  friend void to_json(
      nlohmann::json& json_power_of_tau,
      const Power<TFriendBlstPoint, friend_size_in_bytes>& power_of_tau);
};

static constexpr size_t p1_power_size_in_bytes = 48;
using G1Power = Power<blst::P1, p1_power_size_in_bytes>;

static constexpr size_t p2_power_size_in_bytes = 96;
using G2Power = Power<blst::P2, p2_power_size_in_bytes>;

#endif // POWER_HPP