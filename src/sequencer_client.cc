#include "include/sequencer_client.h"
#include "include/auth_request_link_response.h"
#include "include/contribution_response.h"
#include "include/contribution_schema.h"
#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>

using nlohmann::json;

enum class HttpStatus {
  ok = 200,
  bad_request = 400,
};

static bool is_ok_status(uint64_t status_code) {
  return status_code == static_cast<int64_t>(HttpStatus::ok);
}

static bool is_bad_request_status(uint64_t status_code) {
  return status_code == static_cast<int64_t>(HttpStatus::bad_request);
}

static bool is_rate_limited_response(const cpr::Response& response) {
  if (!is_bad_request_status(response.status_code)) {
    return false;
  }

  const auto json_response = json::parse(response.text);
  const auto code_iter = json_response.find("code");
  if (code_iter == json_response.end()) {
    return false;
  }

  return *code_iter == "TryContributeError::RateLimited";
}

static bool
is_another_contribution_in_progress_response(const cpr::Response& response) {
  if (!is_ok_status(response.status_code)) {
    return false;
  }

  const auto json_response = json::parse(response.text);
  const auto code_iter = json_response.find("code");
  if (code_iter == json_response.end()) {
    return false;
  }

  return *code_iter == "TryContributeError::AnotherContributionInProgress";
}

SequencerClient::SequencerClient(std::string sequencer_url, uint16_t port)
    : sequencer_url_(std::move(sequencer_url)), port_(port),
      auth_request_link_url_(build_auth_request_link_url()),
      contribution_url_(build_contribution_url()),
      ceremony_status_url_(build_ceremony_status_url()),
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

void SequencerClient::print_lobby_size() const {
  const auto response = cpr::Get(cpr::Url{ceremony_status_url_});

  if (is_ok_status(response.status_code)) {
    const auto json_response = json::parse(response.text);
    const auto lobby_size_iter = json_response.find("lobby_size");

    if (lobby_size_iter != json_response.end()) {
      const auto lobby_size = lobby_size_iter->get<uint64_t>();
      std::cout << "Number of people in the lobby: " << lobby_size << std::endl;
    }
  }
}

ContributionResponse
SequencerClient::try_contribute(const std::string& session_id) const {
  bool slot_reserved = false;
  json json_response;

  while (!slot_reserved) {
    const cpr::Url url{contribution_url_};
    cpr::Session contribution_session;
    contribution_session.SetHeader(
        cpr::Header{{"Authorization", "Bearer " + session_id}});
    contribution_session.SetUrl(url);

    const auto response = contribution_session.Post();
    json_response = json::parse(response.text);
    const auto code_iter = json_response.find("code");

    // When authenticated, the spec says that a single IP address can send a
    // maximum of 50 requests per minute, which means that we can send 1 request
    // every 1200 milliseconds. Since we send a maximum of 2 requests every time
    // (/lobby/try_contribute and /info/status), we double this number to make
    // it 2400 milliseconds
    // https://github.com/ethereum/kzg-ceremony-specs/blob/ece8c05725024eb6709f93b5dcf2aec0ab7687ac/docs/participant/lifecycle.md#rate-limiting
    static constexpr uint64_t retry_delay_milliseconds = 2400;

    if (is_rate_limited_response(response)) {
      std::cout << "Rate limited response received. Trying again in "
                << retry_delay_milliseconds << " milliseconds." << std::endl;
      print_lobby_size();

      std::this_thread::sleep_for(
          std::chrono::milliseconds(retry_delay_milliseconds));
    } else if (is_another_contribution_in_progress_response(response)) {
      std::cout << "Another contribution is in progress. Trying again in "
                << retry_delay_milliseconds << " milliseconds." << std::endl;

      print_lobby_size();

      std::this_thread::sleep_for(
          std::chrono::milliseconds(retry_delay_milliseconds));
    } else if (!is_ok_status(response.status_code)) {
      std::stringstream error_stream;
      error_stream << "Error while trying to contribute via "
                   << contribution_url_ << ": " << response.text;
      throw std::runtime_error(error_stream.str());
    } else {
      slot_reserved = true;
    }
  }

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

std::string SequencerClient::build_ceremony_status_url() const {
  return sequencer_url_ + "/info/status";
}