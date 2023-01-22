#ifndef SEQUENCER_CLIENT_HPP
#define SEQUENCER_CLIENT_HPP

#include "auth_request_link.hpp"
#include "batch_contribution.hpp"
#include "include/contribution_receipt.hpp"
#include <nlohmann/json.hpp>
#include <string>

class UnknownSessionIdError : std::exception {};

class SequencerClient {
public:
  SequencerClient(std::string sequencer_url, uint16_t port);
  AuthRequestLink get_auth_request_link() const;
  BatchContribution try_contribute(const std::string& session_id) const;
  ContributionReceipt contribute(
      const std::string& session_id,
      const BatchContribution& batch_contribution) const;

private:
  const std::string sequencer_url_;
  const uint16_t port_;
  const std::string auth_request_link_url_;
  const std::string try_contribute_url_;
  const std::string ceremony_status_url_;
  const std::string contribute_url_;
  const nlohmann::json contribution_schema_json_;

  void print_lobby_size() const;
  std::string build_auth_request_link_url() const;
  std::string build_try_contribute_url() const;
  std::string build_ceremony_status_url() const;
  std::string build_contribute_url() const;
};

#endif // SEQUENCER_CLIENT_HPP