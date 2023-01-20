#include "include/arg_parser.hpp"
#include "include/ascii_title.hpp"
#include "include/auth_callback_server.hpp"
#include "include/auth_info.hpp"
#include "include/auth_request_link_response.hpp"
#include "include/contribution_response.hpp"
#include "include/contribution_schema.hpp"
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

#ifdef _WIN32
#include <shellapi.h>
#else
#include <unistd.h>
#endif

static void authenticate(const std::string& auth_url) {
  std::cout << "Your browser should now open a tab with the authentication "
               "link. If you close the tab by mistake or an error occurred "
               "during the authentication, you can manually open the following "
               "URL in your browser: "
            << std::endl
            << std::endl
            << auth_url << std::endl
            << std::endl;

  auto response = cpr::Get(cpr::Url{auth_url});

#ifdef _WIN32
  ShellExecuteA(0, 0, auth_url.c_str(), 0, 0, SW_SHOW);
#else
  auto pid = fork();
  if (pid == 0) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    execl("/usr/bin/xdg-open", "xdg-open", auth_url.c_str(), nullptr);
  }
#endif
}

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
    const auto port = arg_parser.get_auth_callback_port();

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
          break;
        case AuthProvider::GitHub:
          return auth_request_link.get_github_auth_url();
        }
      }();

      authenticate(auth_url);
      auto auth_future = auth_info_promise.get_future();
      AuthInfo auth_info = auth_future.get();

      if (!auth_info.get_error_message().empty()) {
        throw std::runtime_error(auth_info.get_error_message());
      }

      try {
        // Wait until a contribution slot is available
        auto contribution_response =
            sequencer_client.try_contribute(auth_info.get_session_id());

        // Validate the powers
        contribution_response.validate_powers();

        // Update the powers of Tau with the secrets generated earlier
        std::cout << "Updating powers of Tau... ";
        auto& contributions = contribution_response.get_contributions();
        const auto& secrets = secret_generator.get_secrets();
        auto secret_iter = secrets.begin();
        for (auto& contribution : contributions) {
          contribution.update_powers_of_tau(*secret_iter);
          ++secret_iter;
        }
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