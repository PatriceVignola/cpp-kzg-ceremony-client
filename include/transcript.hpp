#ifndef TRANSCRIPT_HPP
#define TRANSCRIPT_HPP

#include "powers_of_tau.hpp"
#include "witness.hpp"
#include <nlohmann/json.hpp>
#include <string>

class Transcript {
public:
  const Witness& get_witness() const { return witness_; }
  int get_num_g1_powers() const { return num_g1_powers_; }
  int get_num_g2_powers() const { return num_g2_powers_; }

private:
  int num_g1_powers_ = 0;
  int num_g2_powers_ = 0;
  PowersOfTau powers_of_tau_;
  Witness witness_;

  friend void from_json(const nlohmann::json& json_transcript,
                        Transcript& transcript);

  friend void to_json(nlohmann::json& json_transcript,
                      const Transcript& transcript);
};

#endif // TRANSCRIPT_HPP