#ifndef CONTRIBUTION_RECEIPT_HPP
#define CONTRIBUTION_RECEIPT_HPP

#include "receipt.hpp"
#include <nlohmann/json.hpp>
#include <string>

class ContributionReceipt {
public:
  const Receipt& get_receipt() const { return receipt_; }
  const std::string& get_signature() const { return signature_; }

private:
  Receipt receipt_;
  std::string signature_;

  friend void from_json(const nlohmann::json& json_contribution_receipt,
                        ContributionReceipt& contribution_receipt);

  friend void to_json(nlohmann::json& json_contribution_receipt,
                      const ContributionReceipt& contribution_receipt);
};

#endif // CONTRIBUTION_RECEIPT_HPP