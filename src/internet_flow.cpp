#include "include/arg_parser.hpp"
#include "include/ascii_title.hpp"
#include "include/auth_browser.hpp"
#include "include/auth_helpers.hpp"
#include "include/auth_info.hpp"
#include "include/auth_request_link.hpp"
#include "include/bls_signature.hpp"
#include "include/contribution_schema.hpp"
#include "include/identity_fetcher.hpp"
#include "include/port_picker.hpp"
#include "include/sequencer_client.hpp"
#include "include/server.hpp"
#include "include/signing_browser.hpp"
#include <absl/strings/str_cat.h>
#include <cpr/cpr.h>
#include <iostream>

using nlohmann::json;

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

  Server server(port, std::move(auth_callback), nullptr, {});

  bool contribution_successful = false;

  while (!contribution_successful) {
    absl::optional<AuthInfo> auth_info;
    auto session_id = arg_parser.get_session_id();
    auto nickname = arg_parser.get_nickname();

    if (session_id.empty()) {
      auth_info.emplace(auth_helpers::authenticate(
          sequencer_client, auth_provider, auth_info_promise));

      session_id = auth_info->get_session_id();
      nickname = auth_info->get_nickname();
    }

    // Retrieve the identity (e.g. eth|0xa7fb...)
    std::cout << "Retrieving your identity" << std::endl;
    const auto identity = [auth_provider, nickname]() {
      switch (auth_provider) {
      case AuthProvider::Ethereum:
        return identity_fetcher::get_ethereum_identity(nickname);
      case AuthProvider::GitHub:
        return identity_fetcher::get_github_identity(nickname);
      default:
        throw std::runtime_error("Error: unsupported authentication provider");
      }
    }();

    try {
      // Wait until a contribution slot is available
      auto batch_contribution = sequencer_client.try_contribute(session_id);

      // Validate the powers
      batch_contribution.validate_powers();

      std::cout << "Saving the contribution file to `"
                << arg_parser.get_contribution_file_path() << "`" << std::endl;

      {
        std::ofstream batch_contribution_stream(
            arg_parser.get_contribution_file_path());
        if (batch_contribution_stream.fail()) {
          throw std::runtime_error(
              absl::StrCat("failed to save the contribution file to `",
                           arg_parser.get_contribution_file_path(), "`"));
        }

        auto batch_contribution_json = json(batch_contribution);
        batch_contribution_json["identity"] = identity;
        batch_contribution_stream << batch_contribution_json;
      }

      std::cout
          << "The contribution file has been saved to `"
          << arg_parser.get_contribution_file_path()
          << "`. You can now move this file over to an airgapped machine. Once "
             "you are done updating the contribution, overwrite this file with "
             "the updated version and press the `enter` key."
          << std::endl;

      // NOLINTNEXTLINE(cert-err33-c)
      getchar();

      std::cout << "Reading the updated contribution file from `"
                << arg_parser.get_contribution_file_path() << "`" << std::endl;

      {
        std::ifstream json_file_stream(arg_parser.get_contribution_file_path());

        if (json_file_stream.fail()) {
          throw std::runtime_error(absl::StrCat(
              "failed to open the updated contribution file located at `",
              arg_parser.get_contribution_file_path(), "`"));
        }

        batch_contribution = BatchContribution(
            json::parse(json_file_stream), json::parse(contribution_schema));
      }

      // Validate the powers
      batch_contribution.validate_powers();

      std::cout << "Submitting the updated contributions" << std::endl;
      const auto contribution_receipt =
          sequencer_client.contribute(session_id, batch_contribution);

      std::cout << "Your contribution was successfully submitted! Here is "
                   "your contribution receipt:"
                << std::endl;

      static constexpr int json_indent = 4;
      std::cout << nlohmann::json(contribution_receipt).dump(json_indent)
                << std::endl;

      contribution_successful = true;
    } catch (const UnknownSessionIdError& ex) {
      auto error_message =
          absl::StrCat("Session ID `", session_id,
                       "` is invalid or expired. Try authenticating again.");

      if (!arg_parser.get_session_id().empty()) {
        throw std::invalid_argument(error_message);
      }

      std::cout << error_message << std::endl;
      auth_info_promise = std::promise<AuthInfo>();
    }
  }
}

} // namespace internet_flow
