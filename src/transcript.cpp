#include "include/transcript.hpp"

void from_json(const nlohmann::json& json_transcript, Transcript& transcript) {
  json_transcript.at("numG1Powers").get_to(transcript.num_g1_powers_);
  json_transcript.at("numG2Powers").get_to(transcript.num_g2_powers_);
  json_transcript.at("powersOfTau").get_to(transcript.powers_of_tau_);
  json_transcript.at("witness").get_to(transcript.witness_);
}

void to_json(nlohmann::json& json_transcript, const Transcript& transcript) {
  json_transcript["numG1Powers"] = transcript.num_g1_powers_;
  json_transcript["numG2Powers"] = transcript.num_g2_powers_;
  json_transcript["powersOfTau"] = transcript.powers_of_tau_;
  json_transcript["witness"] = transcript.witness_;
}