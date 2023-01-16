#ifndef POWERS_OF_TAU_HPP
#define POWERS_OF_TAU_HPP

#include "g1_power.hpp"
#include "g2_power.hpp"
#include <nlohmann/json.hpp>
#include <vector>

class PowersOfTau {
public:
  const std::vector<G1Power>& get_g1_powers() const;
  const std::vector<G2Power>& get_g2_powers() const;
  bool valid() const;

private:
  std::vector<G1Power> g1_powers;
  std::vector<G2Power> g2_powers;

  friend void from_json(const nlohmann::json& json_powers_of_tau,
                        PowersOfTau& powers_of_tau);
};

#endif // POWERS_OF_TAU_HPP