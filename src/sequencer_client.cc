#include "include/sequencer_client.h"
#include "include/auth_request_link_response.h"
#include "include/contribution_response.h"
#include "include/contribution_schema.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <sstream>

using nlohmann::json;

enum class HttpStatus {
  ok = 200,
};

static bool is_ok_status(uint64_t status_code) {
  return status_code == static_cast<int64_t>(HttpStatus::ok);
}

SequencerClient::SequencerClient(std::string sequencer_url, uint16_t port)
    : sequencer_url_(std::move(sequencer_url)), port_(port),
      auth_request_link_url_(build_auth_request_link_url()),
      contribution_url_(build_contribution_url()),
      contribution_schema_json_(json::parse(contribution_schema)) {}

AuthRequestLinkResponse SequencerClient::get_auth_request_link() const {
  const auto response = cpr::Get(cpr::Url{auth_request_link_url_});
  if (!is_ok_status(response.status_code)) {
    std::stringstream error_stream;
    error_stream << "error " << response.status_code << ": " << response.text;
    throw std::runtime_error(error_stream.str());
  }

  const auto json_response = json::parse(response.text);
  return AuthRequestLinkResponse(json_response);
}

ContributionResponse
SequencerClient::try_contribute(const std::string& session_id) const {
  const cpr::Url url{contribution_url_};
  cpr::Session contribution_session;
  contribution_session.SetHeader(
      cpr::Header{{"Authorization", "Bearer " + session_id}});
  contribution_session.SetUrl(url);

  const auto response = contribution_session.Post();
  if (!is_ok_status(response.status_code)) {
    std::stringstream error_stream;
    error_stream << "Error while trying to contribute via " << contribution_url_
                 << ": " << response.text;
    ;
    throw std::runtime_error(error_stream.str());
  }

  const auto json_response = json::parse(response.text);
  return ContributionResponse(json_response, contribution_schema_json_);
}

std::string SequencerClient::build_auth_request_link_url() const {
  std::stringstream url_stream;
  url_stream << sequencer_url_
             << "/auth/request_link?redirect_to=http://localhost:" << port_
             << "/auth_callback";

  return url_stream.str();
}

std::string SequencerClient::build_contribution_url() const {
  return sequencer_url_ + "/lobby/try_contribute";
}