#ifndef AUTH_REQUEST_LINK_HPP
#define AUTH_REQUEST_LINK_HPP

#include <nlohmann/json.hpp>
#include <string>

class AuthRequestLink {
public:
  const std::string& get_eth_auth_url() const;
  const std::string& get_github_auth_url() const;

private:
  std::string eth_auth_url_;
  std::string github_auth_url_;

  friend void from_json(const nlohmann::json& json_auth_request_link,
                        AuthRequestLink& auth_request_link);
};

#endif // AUTH_REQUEST_LINK_HPP