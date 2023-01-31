#ifndef RECEIPT_HPP
#define RECEIPT_HPP

#include <nlohmann/json.hpp>
#include <string>

class Receipt {
public:
  const std::string& get_identity() const { return identity_; }
  const std::vector<std::string>& get_witness() const { return witness_; }

private:
  std::string identity_;
  std::vector<std::string> witness_;

  friend void from_json(const nlohmann::json& json_receipt, Receipt& receipt);
  friend void to_json(nlohmann::json& json_receipt, const Receipt& receipt);
};

#endif // RECEIPT_HPP