#ifndef CONTRIBUTION_RECEIPT_HPP
#define CONTRIBUTION_RECEIPT_HPP

#include "receipt.hpp"
#include <nlohmann/json.hpp>
#include <string>

class ContributionReceipt {
public:
  const std::string& get_signature() const { return signature_; }
  const Receipt& get_receipt() const { return receipt_; }

private:
  Receipt receipt_;
  std::string signature_;

  friend void from_json(const nlohmann::json& json_contribution_receipt,
                        ContributionReceipt& contribution_receipt);
};

#endif // CONTRIBUTION_RECEIPT_HPP