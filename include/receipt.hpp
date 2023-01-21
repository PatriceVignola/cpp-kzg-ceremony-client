#ifndef RECEIPT_HPP
#define RECEIPT_HPP

#include "id_token.hpp"
#include <nlohmann/json.hpp>
#include <string>

class Receipt {
private:
  IdToken id_token_;
  std::string g2_;

  friend void from_json(const nlohmann::json& json_receipt,
                        Receipt& power_of_tau);
};

#endif // RECEIPT_HPP