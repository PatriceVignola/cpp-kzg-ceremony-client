#ifndef SEQUENCER_CLIENT_HPP
#define SEQUENCER_CLIENT_HPP

#include "auth_request_link_response.hpp"
#include "contribution_response.hpp"
#include <nlohmann/json.hpp>
#include <string>

class UnknownSessionIdError : std::exception {};

class SequencerClient {
public:
  SequencerClient(std::string sequencer_url, uint16_t port);
  AuthRequestLinkResponse get_auth_request_link() const;
  ContributionResponse try_contribute(const std::string& session_id) const;

private:
  const std::string sequencer_url_;
  const uint16_t port_;
  const std::string auth_request_link_url_;
  const std::string contribution_url_;
  const std::string ceremony_status_url_;
  const nlohmann::json contribution_schema_json_;

  void print_lobby_size() const;
  std::string build_auth_request_link_url() const;
  std::string build_contribution_url() const;
  std::string build_ceremony_status_url() const;
};

#endif // SEQUENCER_CLIENT_HPP