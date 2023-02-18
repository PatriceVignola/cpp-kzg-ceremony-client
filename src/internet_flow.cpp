#include "include/arg_parser.hpp"
#include "include/ascii_title.hpp"
#include "include/auth_browser.hpp"
#include "include/auth_info.hpp"
#include "include/auth_request_link.hpp"
#include "include/bls_signature.hpp"
#include "include/identity_fetcher.hpp"
#include "include/port_picker.hpp"
#include "include/sequencer_client.hpp"
#include "include/server.hpp"
#include "include/signing_browser.hpp"
#include <absl/strings/str_cat.h>
#include <cpr/cpr.h>
#include <iostream>

namespace internet_flow {
void launch(const ArgParser& arg_parser) {
  const auto auth_provider = arg_parser.get_auth_provider();

  uint16_t port = 0;
  if (arg_parser.get_port().has_value()) {
    port = *arg_parser.get_port();
  } else {
    port = port_picker::pick_unused_port();
  }

  std::promise<AuthInfo> auth_info_promise;
  auto auth_callback = [&auth_info_promise](AuthInfo&& auth_info) {
    auth_info_promise.set_value(std::move(auth_info));
  };

  const SequencerClient sequencer_client(arg_parser.get_sequencer_url(), port);

  // Retrieve the batch transcript
  std::cout << "Retrieving the transcript" << std::endl;
  const auto batch_transcript = sequencer_client.get_batch_transcript();

  // Validate the running products
  batch_transcript.validate_running_products();

  bool contribution_successful = false;

  while (!contribution_successful) {
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

    // Retrieve the identity (e.g. eth|12345|0xa7fb...)
    std::cout << "Retrieving your identity" << std::endl;
    const auto identity = [&auth_provider, &auth_info]() {
      switch (auth_provider) {
      case AuthProvider::Ethereum:
        return identity_fetcher::get_ethereum_identity(
            auth_info.get_nickname());
      case AuthProvider::GitHub:
        return identity_fetcher::get_github_identity(auth_info.get_nickname());
      default:
        throw std::runtime_error("Error: unsupported authentication provider");
      }
    }();

    try {
      // Wait until a contribution slot is available
      auto batch_contribution =
          sequencer_client.try_contribute(auth_info.get_session_id());

      // Validate the powers
      batch_contribution.validate_powers();

      // TODO: Read contribution.json from hard drive

      std::cout << "Submitting the updated contributions" << std::endl;
      const auto contribution_receipt = sequencer_client.contribute(
          auth_info.get_session_id(), batch_contribution);

      std::cout << "Your contribution was successfully submitted! Here is "
                   "your contribution receipt:"
                << std::endl;

      static constexpr int json_indent = 4;
      std::cout << nlohmann::json(contribution_receipt).dump(json_indent)
                << std::endl;

      contribution_successful = true;
    } catch (const UnknownSessionIdError& ex) {
      std::cout << "Session ID expired. Try authenticating again." << std::endl;
      auth_info_promise = std::promise<AuthInfo>();
    }
  }
}

} // namespace internet_flow
