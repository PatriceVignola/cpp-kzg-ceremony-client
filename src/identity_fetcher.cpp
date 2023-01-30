#include "include/identity_fetcher.hpp"
#include "include/github_user.hpp"
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
std::string get_ethereum_identity(std::string ethereum_address) {
  // TODO (PatriceVignola): Remove when we move to abseil
  // NOLINTNEXTLINE(abseil-string-find-startswith)
  if (ethereum_address.rfind("0x", 0) != 0) {
    throw std::runtime_error("Ethereum address `" + ethereum_address +
                             "` doesn't start with `0x`");
  }

  static constexpr size_t ethereum_address_length = 42;
  if (ethereum_address.length() != ethereum_address_length) {
    throw std::runtime_error("Ethereum address `" + ethereum_address +
                             "` doesn't have 42 characters");
  }

  std::transform(ethereum_address.begin(), ethereum_address.end(),
                 ethereum_address.begin(), ::tolower);

  return "eth|" + ethereum_address;
}

std::string get_github_identity(const std::string& github_nickname) {
  const auto github_users_url =
      "https://api.github.com/users/" + github_nickname;

  const auto response = cpr::Get(cpr::Url{github_users_url});

  if (is_not_found_status(response.status_code)) {
    throw std::runtime_error(github_nickname +
                             " is not a valid GitHub username");
  }

  if (!is_ok_status(response.status_code)) {
    auto error_message = response.error.code != cpr::ErrorCode::OK
                             ? response.error.message
                             : response.text;

    throw std::runtime_error(
        "Error while retrieving the batch transcript via " + github_users_url +
        ": " + error_message);
  }

  const auto json_response = nlohmann::json::parse(response.text);
  const auto github_user = json_response.get<GithubUser>();
  return "git|" + std::to_string(github_user.get_id()) + "|" +
         github_user.get_login();
}
} // namespace identity_fetcher