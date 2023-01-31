#include "include/contribution_receipt.hpp"

void from_json(const nlohmann::json& json_contribution_receipt,
               ContributionReceipt& contribution_receipt) {
  auto serialized_receipt =
      json_contribution_receipt.at("receipt").get<std::string>();

  auto json_receipt = nlohmann::json::parse(serialized_receipt);
  json_receipt.get_to(contribution_receipt.receipt_);

  json_contribution_receipt.at("signature")
      .get_to(contribution_receipt.signature_);
}

void to_json(nlohmann::json& json_contribution_receipt,
             const ContributionReceipt& contribution_receipt) {
  json_contribution_receipt["receipt"] = contribution_receipt.receipt_;
  json_contribution_receipt["signature"] = contribution_receipt.signature_;
}