#include "include/receipt.hpp"

void from_json(const nlohmann::json& json_receipt, Receipt& receipt) {
  json_receipt.at("id_token").get_to(receipt.id_token_);
  json_receipt.at("g2").get_to(receipt.g2_);
}