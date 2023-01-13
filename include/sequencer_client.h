#ifndef SEQUENCER_CLIENT_H
#define SEQUENCER_CLIENT_H

#include "auth_request_link_response.h"
#include "contribution_response.h"
#include <nlohmann/json.hpp>
#include <string>

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
  const nlohmann::json contribution_schema_json_;

  std::string build_auth_request_link_url() const;
  std::string build_contribution_url() const;
};

#endif // SEQUENCER_CLIENT_H