#ifndef CONTRIBUTION_HPP
#define CONTRIBUTION_HPP

#include "powers_of_tau.hpp"
#include <nlohmann/json.hpp>
#include <string>

#ifdef _DLL
#undef _DLL
#include <uint256_t.h>
#define _DLL
#else
#include <uint256_t.h>
#endif

class Contribution {
public:
  int get_num_g1_powers() const { return num_g1_powers_; }
  int get_num_g2_powers() const { return num_g2_powers_; }
  const PowersOfTau& get_powers_of_tau() const { return powers_of_tau_; }
  const std::string& get_pot_pubkey() const { return pot_pubkey_; }
  const std::string& get_bls_signature() const { return bls_signature_; }
  void update_powers_of_tau(uint256_t secret);

private:
  int num_g1_powers_;
  int num_g2_powers_;
  PowersOfTau powers_of_tau_;
  std::string pot_pubkey_;
  std::string bls_signature_;

  friend void from_json(const nlohmann::json& json_contribution,
                        Contribution& contribution);
};

#endif // CONTRIBUTION_HPP