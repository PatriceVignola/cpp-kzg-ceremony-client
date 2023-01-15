#include "include/auth_request_link_response.hpp"
#include <nlohmann/json.hpp>

AuthRequestLinkResponse::AuthRequestLinkResponse(
    const nlohmann::json& json_response) {

  if (json_response.find("eth_auth_url") == json_response.end()) {
    throw std::runtime_error("field `eth_auth_url` not found");
  }

  if (json_response.find("github_auth_url") == json_response.end()) {
    throw std::runtime_error("field `github_auth_url` not found");
  }

  eth_auth_url_ = json_response["eth_auth_url"];
  github_auth_url_ = json_response["github_auth_url"];
}

const std::string& AuthRequestLinkResponse::get_eth_auth_url() const {
  return eth_auth_url_;
}
const std::string& AuthRequestLinkResponse::get_github_auth_url() const {
  return github_auth_url_;
}
