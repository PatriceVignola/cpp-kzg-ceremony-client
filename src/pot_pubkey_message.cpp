#include "include/pot_pubkey_message.hpp"
#include <nlohmann/json.hpp>

PotPubkeyMessage::PotPubkeyMessage(int num_g1_powers, int num_g2_powers,
                                   std::string pot_pubkey)
    : num_g1_powers_(num_g1_powers), num_g2_powers_(num_g2_powers),
      pot_pubkey_(std::move(pot_pubkey)) {}

void to_json(nlohmann::json& json_pot_pubkey_message,
             const PotPubkeyMessage& pot_pubkey_message) {
  json_pot_pubkey_message["numG1Powers"] = pot_pubkey_message.num_g1_powers_;
  json_pot_pubkey_message["numG2Powers"] = pot_pubkey_message.num_g2_powers_;
  json_pot_pubkey_message["potPubkey"] = pot_pubkey_message.pot_pubkey_;
}
