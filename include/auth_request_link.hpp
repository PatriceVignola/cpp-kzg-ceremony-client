#ifndef AUTH_REQUEST_LINK_HPP
#define AUTH_REQUEST_LINK_HPP

#include <absl/strings/string_view.h>
#include <nlohmann/json.hpp>
#include <string>

class AuthRequestLink {
public:
  absl::string_view get_eth_auth_url() const;
  absl::string_view get_github_auth_url() const;

private:
  std::string eth_auth_url_;
  std::string github_auth_url_;

  friend void from_json(const nlohmann::json& json_auth_request_link,
                        AuthRequestLink& auth_request_link);
};

#endif // AUTH_REQUEST_LINK_HPP