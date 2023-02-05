#ifndef POWERS_OF_TAU_HPP
#define POWERS_OF_TAU_HPP

#include "power.hpp"
#include <absl/types/span.h>
#include <nlohmann/json.hpp>
#include <vector>

class PowersOfTau {
public:
  absl::Span<G1Power> get_g1_powers();
  absl::Span<G2Power> get_g2_powers();
  absl::Span<const G1Power> get_g1_powers() const;
  absl::Span<const G2Power> get_g2_powers() const;
  void multiply_g1_power(int power_index, absl::Span<const uint8_t> power);
  void multiply_g2_power(int power_index, absl::Span<const uint8_t> power);
  bool valid() const;

private:
  std::vector<G1Power> g1_powers_;
  std::vector<G2Power> g2_powers_;

  friend void from_json(const nlohmann::json& json_powers_of_tau,
                        PowersOfTau& powers_of_tau);

  friend void to_json(nlohmann::json& json_powers_of_tau,
                      const PowersOfTau& powers_of_tau);
};

#endif // POWERS_OF_TAU_HPP