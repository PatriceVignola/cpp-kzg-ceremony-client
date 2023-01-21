#ifndef ID_TOKEN_HPP
#define ID_TOKEN_HPP

#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>

class IdToken {
private:
  std::string sub_;
  std::string nickname_;
  std::string provider_;
  uint64_t exp_;

  friend void from_json(const nlohmann::json& json_id_token, IdToken& id_token);
};

#endif // ID_TOKEN_HPP