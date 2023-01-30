#ifndef POT_PUBKEY_MESSAGE_HPP
#define POT_PUBKEY_MESSAGE_HPP

#include <nlohmann/json.hpp>
#include <string>

class PotPubkeyMessage {
public:
  PotPubkeyMessage(int num_g1_powers, int num_g2_powers,
                   std::string pot_pubkey);

private:
  int num_g1_powers_;
  int num_g2_powers_;
  std::string pot_pubkey_;

  friend void to_json(nlohmann::json& json_pot_pubkey_message,
                      const PotPubkeyMessage& pot_pubkey_message);
};

#endif // POT_PUBKEY_MESSAGE_HPP