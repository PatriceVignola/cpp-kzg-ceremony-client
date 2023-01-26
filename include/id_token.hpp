#ifndef ID_TOKEN_HPP
#define ID_TOKEN_HPP

#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>

class IdToken {
public:
  const std::string& get_sub() const { return sub_; }
  const std::string& get_nickname() const { return nickname_; }
  const std::string& get_provider() const { return provider_; }
  uint64_t get_exp() const { return exp_; }

private:
  std::string sub_;
  std::string nickname_;
  std::string provider_;
  uint64_t exp_;

  friend void from_json(const nlohmann::json& json_id_token, IdToken& id_token);
};

#endif // ID_TOKEN_HPP