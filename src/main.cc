#include "include/arg_parser.h"
#include "include/ascii_title.h"
#include "include/auth_callback_server.h"
#include "include/auth_info.h"
#include "include/auth_request_link_response.h"
#include "include/contribution_response.h"
#include "include/contribution_schema.h"
#include "include/sequencer_client.h"
#include <blst.hpp>
#include <cpr/cpr.h>
#include <iostream>
#include <unistd.h>

static void authenticate(const std::string& auth_url) {
  std::cout << "Your browser should now open a tab with the authentication "
               "link. If you close the tab by mistake or an error occurred "
               "during the authentication, you can manually go to the "
               "following URL in your browser: "
            << std::endl
            << std::endl
            << auth_url << std::endl
            << std::endl;

  auto response = cpr::Get(cpr::Url{auth_url});

  auto pid = fork();
  if (pid == 0) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    execl("/usr/bin/xdg-open", "xdg-open", auth_url.c_str(), nullptr);
  }
}

int main(int argc, char** argv) {
  try {
    const ArgParser arg_parser(argc, argv);
    if (arg_parser.get_help_wanted()) {
      std::cout << arg_parser.get_help_message() << std::endl;
      return 0;
    }

    const auto& auth_provider = arg_parser.get_auth_provider();

    std::promise<AuthInfo> auth_info_promise;

    const auto port = arg_parser.get_auth_callback_port();
    AuthCallbackServer auth_callback_server(
        port, [&auth_info_promise](AuthInfo&& auth_info) {
          auth_info_promise.set_value(std::move(auth_info));
        });

    const SequencerClient sequencer_client(arg_parser.get_sequencer_url(),
                                           port);

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

    const auto contribution_response =
        sequencer_client.try_contribute(auth_info.get_session_id());
    std::cout << "The contribution file received from the sequencer was "
                 "successfully validated against the schema!"
              << std::endl;

    contribution_response.validate_powers();
    std::cout << "All powers of Tau received from the sequencer were "
                 "successfully validated!"
              << std::endl;
  } catch (const std::exception& ex) {
    std::cout << ex.what() << std::endl;
    return 1;
  }

  return 0;
}