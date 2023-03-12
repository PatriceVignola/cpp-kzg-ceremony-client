#include "include/auth_helpers.hpp"
#include "include/auth_browser.hpp"
#include "include/sequencer_client.hpp"

namespace auth_helpers {
AuthInfo authenticate(const SequencerClient& sequencer_client,
                      AuthProvider auth_provider,
                      std::promise<AuthInfo>& auth_info_promise) {
  const auto auth_request_link = sequencer_client.get_auth_request_link();

  const auto auth_url = [&auth_provider, &auth_request_link]() {
    switch (auth_provider) {
    case AuthProvider::Ethereum:
      return auth_request_link.get_eth_auth_url();
    case AuthProvider::GitHub:
      return auth_request_link.get_github_auth_url();
    default:
      throw std::runtime_error("Error: unsupported authentication provider");
    }
  }();

  AuthBrowser auth_browser((std::string(auth_url)));
  auto auth_future = auth_info_promise.get_future();
  AuthInfo auth_info = auth_future.get();

  if (!auth_info.get_error_message().empty()) {
    throw std::runtime_error(auth_info.get_error_message());
  }

  return auth_info;
}
} // namespace auth_helpers