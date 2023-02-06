#include "include/identity_fetcher.hpp"
#include "include/github_user.hpp"
#include <absl/strings/match.h>
#include <absl/strings/str_cat.h>
#include <absl/strings/string_view.h>
#include <algorithm>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

enum class HttpStatus {
  ok = 200,
  not_found = 404,
};

static bool is_ok_status(uint64_t status_code) {
  return status_code == static_cast<int64_t>(HttpStatus::ok);
}

static bool is_not_found_status(uint64_t status_code) {
  return status_code == static_cast<int64_t>(HttpStatus::not_found);
}

namespace identity_fetcher {
std::string get_ethereum_identity(absl::string_view ethereum_address) {
  if (absl::StartsWith(ethereum_address, "0x")) {
    throw std::runtime_error(absl::StrCat(
        "Ethereum address `", ethereum_address, "` doesn't start with `0x`"));
  }

  static constexpr size_t ethereum_address_length = 42;
  if (ethereum_address.length() != ethereum_address_length) {
    throw std::runtime_error(absl::StrCat("Ethereum address `",
                                          ethereum_address,
                                          "` doesn't have 42 characters"));
  }

  std::string lower_case_ethereum_address;
  lower_case_ethereum_address.reserve(ethereum_address.size());

  std::transform(ethereum_address.begin(), ethereum_address.end(),
                 std::back_inserter(lower_case_ethereum_address), ::tolower);

  return absl::StrCat("eth|", ethereum_address);
}

std::string get_github_identity(absl::string_view github_nickname) {
  const auto github_users_url =
      absl::StrCat("https://api.github.com/users/", github_nickname);

  const auto response = cpr::Get(cpr::Url{github_users_url});

  if (is_not_found_status(response.status_code)) {
    throw std::runtime_error(
        absl::StrCat(github_nickname, " is not a valid GitHub username"));
  }

  if (!is_ok_status(response.status_code)) {
    auto error_message = response.error.code != cpr::ErrorCode::OK
                             ? response.error.message
                             : response.text;

    throw std::runtime_error(
        absl::StrCat("Error while retrieving the batch transcript via ",
                     github_users_url, ": ", error_message));
  }

  const auto json_response = nlohmann::json::parse(response.text);
  const auto github_user = json_response.get<GithubUser>();
  return absl::StrCat("git|", github_user.get_id(), "|",
                      github_user.get_login());
}
} // namespace identity_fetcher