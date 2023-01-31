#include "include/receipt.hpp"

void from_json(const nlohmann::json& json_receipt, Receipt& receipt) {
  json_receipt.at("identity").get_to(receipt.identity_);
  json_receipt.at("witness").get_to(receipt.witness_);
}

void to_json(nlohmann::json& json_receipt, const Receipt& receipt) {
  json_receipt["identity"] = receipt.identity_;
  json_receipt["witness"] = receipt.witness_;
}