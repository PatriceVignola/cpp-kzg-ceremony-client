#ifndef POWERS_OF_TAU_HPP
#define POWERS_OF_TAU_HPP

#include "g1_power.hpp"
#include "g2_power.hpp"
#include <nlohmann/json.hpp>
#include <vector>
#ifdef _DLL
#undef _DLL
#include <uint256_t.h>
#define _DLL
#else
#include <uint256_t.h>
#endif

class PowersOfTau {
public:
  std::vector<G1Power>& get_g1_powers();
  std::vector<G2Power>& get_g2_powers();
  void multiply_g1_power(int power_index, uint256_t power);
  void multiply_g2_power(int power_index, uint256_t power);
  bool valid() const;

private:
  std::vector<G1Power> g1_powers;
  std::vector<G2Power> g2_powers;

  friend void from_json(const nlohmann::json& json_powers_of_tau,
                        PowersOfTau& powers_of_tau);
};

#endif // POWERS_OF_TAU_HPP