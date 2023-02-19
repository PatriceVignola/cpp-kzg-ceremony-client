#include "include/sequencer_client.hpp"
#include "include/auth_request_link.hpp"
#include "include/batch_contribution.hpp"
#include "include/batch_transcript.hpp"
#include "include/contribution_error.hpp"
#include "include/contribution_receipt.hpp"
#include "include/contribution_schema.hpp"
#include "include/transcript_schema.hpp"
#include <absl/strings/str_cat.h>
#include <absl/strings/string_view.h>
#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>

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

static bool is_another_contribution_in_progress_response(
    const cpr::Response& response) {
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

SequencerClient::SequencerClient(absl::string_view sequencer_url, uint16_t port)
    : sequencer_url_(sequencer_url), port_(port),
      auth_request_link_url_(build_auth_request_link_url()),
      try_contribute_url_(build_try_contribute_url()),
      contribute_url_(build_contribute_url()),
      ceremony_status_url_(build_ceremony_status_url()),
      current_state_url_(build_current_state_url()) {}

AuthRequestLink SequencerClient::get_auth_request_link() const {
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

  return json_response.get<AuthRequestLink>();
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

BatchContribution SequencerClient::try_contribute(
    absl::string_view session_id) const {
  bool slot_reserved = false;
  json json_response;

  while (!slot_reserved) {
    // Try to contribute
    const cpr::Url url{try_contribute_url_};
    cpr::Session contribution_session;
    contribution_session.SetHeader(
        cpr::Header{{"Authorization", absl::StrCat("Bearer ", session_id)}});
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
        auto error_message = response.error.code != cpr::ErrorCode::OK
                                 ? response.error.message
                                 : response.text;

        std::cout << "Error while trying to contribute via "
                  << try_contribute_url_ << ": " << error_message
                  << ". We will keep trying to contribute." << std::endl;
      } else {
        json_response = json::parse(response.text);
        slot_reserved = true;
      }
    }

    // Print the number of participants in the lobby
    print_lobby_size();

    // We send a request approximately every second
    static constexpr uint64_t seconds_between_requests = 5;
    std::this_thread::sleep_for(std::chrono::seconds(seconds_between_requests));
  }

  return {json_response, json::parse(contribution_schema)};
}

BatchTranscript SequencerClient::get_batch_transcript() const {
  // Retrieve the batch transcript
  const auto response = cpr::Get(cpr::Url{current_state_url_});

  if (!is_ok_status(response.status_code)) {
    auto error_message = response.error.code != cpr::ErrorCode::OK
                             ? response.error.message
                             : response.text;

    throw std::runtime_error(
        absl::StrCat("Error while retrieving the batch transcript via ",
                     current_state_url_, ": ", error_message));
  }

  const auto json_response = json::parse(response.text);
  return {json_response, json::parse(transcript_schema)};
}

ContributionReceipt SequencerClient::contribute(
    absl::string_view session_id,
    const BatchContribution& batch_contribution) const {
  const cpr::Url url{contribute_url_};
  cpr::Session contribute_session;
  contribute_session.SetHeader(cpr::Header{
      {"Authorization", absl::StrCat("Bearer ", session_id)},
      {"Content-Type", "application/json"},
  });
  contribute_session.SetUrl(url);

  nlohmann::json json_batch_contribution(batch_contribution);
  auto serialized_batch_contribution = json_batch_contribution.dump();
  contribute_session.SetBody(cpr::Body(serialized_batch_contribution));

  const auto response = contribute_session.Post();
  if (is_bad_request_status(response.status_code)) {
    const auto json_response = json::parse(response.text);
    const auto contribution_error = json_response.get<ContributionError>();

    throw std::invalid_argument(
        absl::StrCat("Error `", contribution_error.get_code(),
                     "` while sending a contribution via ", contribute_url_,
                     ": ", contribution_error.get_error()));
  }

  if (!is_ok_status(response.status_code)) {
    auto error_message = response.error.code != cpr::ErrorCode::OK
                             ? response.error.message
                             : response.text;

    throw std::runtime_error(
        absl::StrCat("Error while sending a contribution via ", contribute_url_,
                     ": ", error_message));
  }

  return json::parse(response.text);
}

std::string SequencerClient::build_auth_request_link_url() const {
  return absl::StrCat(sequencer_url_,
                      "/auth/request_link?redirect_to=http://localhost:", port_,
                      "/auth_callback");
}

std::string SequencerClient::build_try_contribute_url() const {
  return absl::StrCat(sequencer_url_, "/lobby/try_contribute");
}

std::string SequencerClient::build_ceremony_status_url() const {
  return absl::StrCat(sequencer_url_, "/info/status");
}

std::string SequencerClient::build_contribute_url() const {
  return absl::StrCat(sequencer_url_, "/contribute");
}

std::string SequencerClient::build_current_state_url() const {
  return absl::StrCat(sequencer_url_, "/info/current_state");
}