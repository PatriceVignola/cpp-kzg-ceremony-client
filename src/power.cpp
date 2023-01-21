#include "include/power.hpp"
#include "include/hex_util.hpp"

#ifdef _DLL
#undef _DLL
#include <uint256_t.h>
#define _DLL
#else
#include <uint256_t.h>
#endif

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
  // TODO (PatriceVignola): Use an std::array instead when hex_util supports
  // spans
  std::vector<uint8_t> serialized_power(size_in_bytes);
  power_of_tau.blst_point_.serialize(serialized_power.data());
  json_power_of_tau = hex_util::encode(serialized_power);
}

template <typename TBlstPoint, size_t size_in_bytes>
void Power<TBlstPoint, size_in_bytes>::multiply(uint256_t power) {
  static constexpr size_t num_bits = 256;
  const auto* power_bytes =
      static_cast<const uint8_t*>(static_cast<void*>(&power));
  blst_point_.mult(power_bytes, num_bits);
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