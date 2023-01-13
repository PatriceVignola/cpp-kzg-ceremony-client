#ifndef AUTH_REQUEST_LINK_RESPONSE_H
#define AUTH_REQUEST_LINK_RESPONSE_H

#include <nlohmann/json.hpp>
#include <string>

class AuthRequestLinkResponse {
public:
  explicit AuthRequestLinkResponse(const nlohmann::json& json_response);

  const std::string& get_eth_auth_url() const;
  const std::string& get_github_auth_url() const;

private:
  std::string eth_auth_url_;
  std::string github_auth_url_;
};

#endif // AUTH_REQUEST_LINK_RESPONSE_H