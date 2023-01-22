#include "include/arg_parser.hpp"
#include "include/ascii_title.hpp"
#include "include/auth_browser.hpp"
#include "include/auth_callback_server.hpp"
#include "include/auth_info.hpp"
#include "include/auth_request_link.hpp"
#include "include/port_picker.hpp"
#include "include/secret_generator.hpp"
#include "include/sequencer_client.hpp"
#include <blst.hpp>
#include <cpr/cpr.h>
#include <iostream>

#ifdef _DLL
#undef _DLL
#include <uint256_t.h>
#define _DLL
#else
#include <uint256_t.h>
#endif

int main(int argc, char** argv) {
  try {
    const ArgParser arg_parser(argc, argv);
    if (arg_parser.get_help_wanted()) {
      std::cout << arg_parser.get_help_message() << std::endl;
      return 0;
    }

    // Generate one secret for each contribution
    std::cout << "Generating secrets... ";
    static constexpr size_t num_secrets = 4;
    SecretGenerator secret_generator(arg_parser.get_entropy(), num_secrets);
    std::cout << "Done!" << std::endl;

    const auto& auth_provider = arg_parser.get_auth_provider();
    const auto port = port_picker::pick_unused_port();

    std::promise<AuthInfo> auth_info_promise;
    AuthCallbackServer auth_callback_server(
        port, [&auth_info_promise](AuthInfo&& auth_info) {
          auth_info_promise.set_value(std::move(auth_info));
        });

    const SequencerClient sequencer_client(arg_parser.get_sequencer_url(),
                                           port);

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
          throw std::runtime_error(
              "Error: unsupported authentication provider");
        }
      }();

      AuthBrowser auth_browser(auth_url);
      auto auth_future = auth_info_promise.get_future();
      AuthInfo auth_info = auth_future.get();

      if (!auth_info.get_error_message().empty()) {
        throw std::runtime_error(auth_info.get_error_message());
      }

      try {
        // Wait until a contribution slot is available
        auto batch_contribution =
            sequencer_client.try_contribute(auth_info.get_session_id());

        // Validate the powers
        batch_contribution.validate_powers();

        // Update the powers of Tau with the secrets generated earlier
        std::cout << "Updating powers of Tau... ";
        auto& contributions = batch_contribution.get_contributions();
        const auto& secrets = secret_generator.get_secrets();
        auto secret_iter = secrets.begin();
        for (auto& contribution : contributions) {
          contribution.update_powers_of_tau(*secret_iter);
          ++secret_iter;
        }
        std::cout << "Done!" << std::endl;

        std::cout << "Submitting the updated contributions... ";
        sequencer_client.contribute(auth_info.get_session_id(),
                                    batch_contribution);
        std::cout << "Done!" << std::endl;

        contribution_successful = true;
      } catch (const UnknownSessionIdError& ex) {
        std::cout << "Session ID expired. Try authenticating again."
                  << std::endl;
        auth_info_promise = std::promise<AuthInfo>();
      }
    }
  } catch (const std::exception& ex) {
    std::cout << ex.what() << std::endl;
    return 1;
  }

  return 0;
}