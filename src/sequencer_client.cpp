#include "include/sequencer_client.hpp"
#include "include/auth_request_link_response.hpp"
#include "include/contribution_response.hpp"
#include "include/contribution_schema.hpp"
#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>

using nlohmann::json;

enum class HttpStatus {
  ok = 200,
  bad_request = 400,
  unauthorized_access = 401,
};

static bool is_ok_status(uint64_t status_code) {
  return status_code == static_cast<int64_t>(HttpStatus::ok);
}

static bool is_bad_request_status(uint64_t status_code) {
  return status_code == static_cast<int64_t>(HttpStatus::bad_request);
}

static bool is_unauthorized_access_status(uint64_t status_code) {
  return status_code == static_cast<int64_t>(HttpStatus::unauthorized_access);
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

static bool is_unknown_session_id_response(const cpr::Response& response) {
  if (!is_unauthorized_access_status(response.status_code)) {
    return false;
  }

  const auto json_response = json::parse(response.text);
  const auto code_iter = json_response.find("code");
  if (code_iter == json_response.end()) {
    return false;
  }

  return *code_iter == "TryContributeError::UnknownSessionId";
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
  uint64_t status_code = 0;
  json json_response;

  const constexpr uint64_t retry_delay_in_seconds = 2;

  while (!is_ok_status(status_code)) {
    const auto response = cpr::Get(cpr::Url{auth_request_link_url_});
    status_code = response.status_code;

    if (response.error.code != cpr::ErrorCode::OK) {
      std::cout << "Error when requesting the authentication link: "
                << response.error.message << ". Trying again in "
                << retry_delay_in_seconds << " seconds." << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(retry_delay_in_seconds));
    } else if (!is_ok_status(status_code)) {
      std::cout << "Received status code `" << response.status_code
                << "` when requesting the authentication link. Trying again in "
                << retry_delay_in_seconds << " seconds." << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(retry_delay_in_seconds));
    } else {
      json_response = json::parse(response.text);
    }
  }

  return AuthRequestLinkResponse(json_response);
}

void SequencerClient::print_lobby_size() const {
  const auto response = cpr::Get(cpr::Url{ceremony_status_url_});

  if (is_ok_status(response.status_code)) {
    const auto json_response = json::parse(response.text);
    const auto lobby_size_iter = json_response.find("lobby_size");

    if (lobby_size_iter != json_response.end()) {
      const auto lobby_size = lobby_size_iter->get<uint64_t>();
      std::cout << "Number of participants in the lobby: " << lobby_size
                << std::endl;
    }
  }
}

ContributionResponse
SequencerClient::try_contribute(const std::string& session_id) const {
  bool slot_reserved = false;
  json json_response;

  while (!slot_reserved) {
    // Try to contribute
    const cpr::Url url{contribution_url_};
    cpr::Session contribution_session;
    contribution_session.SetHeader(
        cpr::Header{{"Authorization", "Bearer " + session_id}});
    contribution_session.SetUrl(url);

    const auto response = contribution_session.Post();
    if (is_unknown_session_id_response(response)) {
      throw UnknownSessionIdError();
    }

    if (is_another_contribution_in_progress_response(response)) {
      std::cout << "A contribution was attempted but another participant is "
                   "currently contributing. Don't worry, we will keep trying "
                   "to contribute!"
                << std::endl;
    } else if (!is_rate_limited_response(response)) {
      if (!is_ok_status(response.status_code)) {
        std::cout << "Error while trying to contribute via "
                  << contribution_url_ << ": " << response.text
                  << ". We will keep trying to contribute." << std::endl;

      } else {
        json_response = json::parse(response.text);
        slot_reserved = true;
      }
    }

    // Print the number of participants in the lobby
    print_lobby_size();

    // We send a request approximately every second
    static constexpr uint64_t seconds_between_requests = 1;
    std::this_thread::sleep_for(std::chrono::seconds(seconds_between_requests));
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