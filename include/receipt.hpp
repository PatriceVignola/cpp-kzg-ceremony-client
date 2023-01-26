#ifndef RECEIPT_HPP
#define RECEIPT_HPP

#include "id_token.hpp"
#include <nlohmann/json.hpp>
#include <string>

class Receipt {
public:
  const IdToken& get_id_token() const { return id_token_; }
  const std::string& get_g2() const { return g2_; }

private:
  IdToken id_token_;
  std::string g2_;

  friend void from_json(const nlohmann::json& json_receipt, Receipt& receipt);
};

#endif // RECEIPT_HPP