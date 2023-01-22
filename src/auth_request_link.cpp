#include "include/auth_request_link.hpp"
#include <nlohmann/json.hpp>

void from_json(const nlohmann::json& json_auth_request_link,
               AuthRequestLink& auth_request_link) {
  json_auth_request_link.at("eth_auth_url")
      .get_to(auth_request_link.eth_auth_url_);
  json_auth_request_link.at("github_auth_url")
      .get_to(auth_request_link.github_auth_url_);
}

const std::string& AuthRequestLink::get_eth_auth_url() const {
  return eth_auth_url_;
}
const std::string& AuthRequestLink::get_github_auth_url() const {
  return github_auth_url_;
}
