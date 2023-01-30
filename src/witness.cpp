#include "include/witness.hpp"

void from_json(const nlohmann::json& json_witness, Witness& witness) {
  json_witness.at("runningProducts").get_to(witness.running_products_);
  json_witness.at("potPubkeys").get_to(witness.pot_pubkeys_);
  json_witness.at("bls_signature").get_to(witness.bls_signatures_);
}

void to_json(nlohmann::json& json_witness, const Witness& witness) {
  json_witness["runningProducts"] = witness.running_products_;
  json_witness["potPubkeys"] = witness.pot_pubkeys_;
  json_witness["bls_signature"] = witness.bls_signatures_;
}

bool Witness::valid() const {
  return std::all_of(
      running_products_.begin(), running_products_.end(),
      [](const auto& running_product) { return running_product.in_group(); });
}