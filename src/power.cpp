#include "include/power.hpp"
#include "include/hex_util.hpp"
#include <iostream>

template <typename TBlstPoint, size_t size_in_bytes>
void from_json(const nlohmann::json& json_power_of_tau,
               Power<TBlstPoint, size_in_bytes>& power_of_tau) {
  const auto power = json_power_of_tau.get<std::string>();
  const auto power_bytes = hex_util::decode(power);
  power_of_tau.blst_point_ = TBlstPoint(power_bytes.data(), power_bytes.size());
}

template <typename TBlstPoint, size_t size_in_bytes>
void to_json(nlohmann::json& json_power_of_tau,
             const Power<TBlstPoint, size_in_bytes>& power_of_tau) {
  json_power_of_tau = power_of_tau.encode();
}

template <typename TBlstPoint, size_t size_in_bytes>
void Power<TBlstPoint, size_in_bytes>::multiply(blst::Scalar scalar) {
  blst_point_.mult(scalar);
}

template <typename TBlstPoint, size_t size_in_bytes>
std::string Power<TBlstPoint, size_in_bytes>::encode() const {
  std::array<uint8_t, size_in_bytes> serialized_power{};
  blst_point_.compress(serialized_power.data());
  return hex_util::encode(serialized_power);
}

template <typename TBlstPoint, size_t size_in_bytes>
bool Power<TBlstPoint, size_in_bytes>::in_group() const {
  return blst_point_.in_group();
}

template class Power<blst::P1, p1_power_size_in_bytes>;
template class Power<blst::P2, p2_power_size_in_bytes>;
template void from_json(const nlohmann::json& json_power_of_tau,
                        G1Power& power_of_tau);
template void from_json(const nlohmann::json& json_power_of_tau,
                        G2Power& power_of_tau);
template void to_json(nlohmann::json& json_power_of_tau,
                      const G1Power& power_of_tau);
template void to_json(nlohmann::json& json_power_of_tau,
                      const G2Power& power_of_tau);