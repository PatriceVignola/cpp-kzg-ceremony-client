#ifndef WITNESS_HPP
#define WITNESS_HPP

#include "power.hpp"
#include <nlohmann/json.hpp>
#include <string>

class Witness {
public:
  bool valid() const;

private:
  std::vector<G1Power> running_products_;
  std::vector<std::string> pot_pubkeys_;
  std::vector<std::string> bls_signatures_;
  friend void from_json(const nlohmann::json& json_witness, Witness& witness);
  friend void to_json(nlohmann::json& json_witness, const Witness& witness);
};

#endif // WITNESS_HPP