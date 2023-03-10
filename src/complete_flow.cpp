#include "include/arg_parser.hpp"
#include "include/ascii_title.hpp"
#include "include/auth_browser.hpp"
#include "include/auth_helpers.hpp"
#include "include/auth_info.hpp"
#include "include/auth_request_link.hpp"
#include "include/bls_signature.hpp"
#include "include/identity_fetcher.hpp"
#include "include/port_picker.hpp"
#include "include/secret_generator.hpp"
#include "include/sequencer_client.hpp"
#include "include/server.hpp"
#include "include/signing_browser.hpp"
#include <absl/strings/str_cat.h>
#include <cpr/cpr.h>
#include <iostream>

namespace complete_flow {
void launch(const ArgParser& arg_parser) {
  // Generate one secret for each contribution
  std::cout << "Generating secrets" << std::endl;
  static constexpr size_t num_secrets = 4;
  SecretGenerator<> secret_generator(arg_parser.get_entropy(), num_secrets);

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

  // Generate the pot pubkeys
  std::vector<std::string> pot_pubkeys;
  pot_pubkeys.reserve(secret_generator.get_secrets().size());

  std::cout << "Generating the pot pubkeys" << std::endl;
  // Sign the identity with the secrets
  for (const auto& secret : secret_generator.get_secrets()) {
    auto pot_pubkey = G2Power::generate_pot_pubkey(secret);
    pot_pubkeys.push_back(pot_pubkey.encode());
  }

  std::promise<std::string> ecdsa_signature_promise;
  auto ecdsa_signature_callback =
      [&ecdsa_signature_promise](std::string&& signature) {
        ecdsa_signature_promise.set_value(std::move(signature));
      };

  std::vector<PotPubkeyMessage> pot_pubkey_messages;
  pot_pubkey_messages.reserve(pot_pubkeys.size());

  const auto& transcripts = batch_transcript.get_transcripts();
  for (size_t i = 0; i < pot_pubkeys.size(); ++i) {
    const auto& pot_pubkey = pot_pubkeys[i];
    const auto& transcript = transcripts[i];
    int num_g1_powers = transcript.get_num_g1_powers();
    int num_g2_powers = transcript.get_num_g2_powers();
    pot_pubkey_messages.emplace_back(num_g1_powers, num_g2_powers, pot_pubkey);
  }

  Server server(port, std::move(auth_callback), ecdsa_signature_callback,
                pot_pubkey_messages);

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

      std::cout << "Authentication successful!" << std::endl;
      std::cout << "sessionid: " << session_id << std::endl;
      std::cout << "nickname: " << nickname << std::endl;
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

    std::string ecdsa_signature;
    if (auth_provider == AuthProvider::Ethereum &&
        !arg_parser.signing_disabled() && ecdsa_signature.empty()) {
      const auto signing_url = absl::StrCat("http://localhost:", port,
                                            "/sign?eth_address=", nickname);

      SigningBrowser signing_browser(signing_url);
      auto ecdsa_signature_future = ecdsa_signature_promise.get_future();
      ecdsa_signature = ecdsa_signature_future.get();
    }

    std::vector<std::string> bls_signatures;
    bls_signatures.reserve(secret_generator.get_secrets().size());

    // Sign the identity with the secrets
    for (const auto& secret : secret_generator.get_secrets()) {
      auto pot_pubkey = G2Power::generate_pot_pubkey(secret);
      pot_pubkeys.push_back(pot_pubkey.encode());

      BlsSignature bls_signature(secret, identity);
      bls_signatures.push_back(bls_signature.encode());
    }

    try {
      // Wait until a contribution slot is available
      auto batch_contribution = sequencer_client.try_contribute(session_id);

      // Validate the powers
      batch_contribution.validate_powers();

      // Update the powers of Tau with the secrets generated earlier
      std::cout << "Updating the contributions" << std::endl;
      auto contributions = batch_contribution.get_contributions();
      const auto secrets = secret_generator.get_secrets();

      for (size_t i = 0; i < contributions.size(); ++i) {
        auto& contribution = contributions[i];
        contribution.update_powers_of_tau(secrets[i]);
        contribution.set_pot_pubkey(std::move(pot_pubkeys[i]));
        contribution.set_bls_signature(std::move(bls_signatures[i]));
      }

      if (!ecdsa_signature.empty()) {
        std::cout << "Setting the ecdsa signature" << std::endl;
        batch_contribution.set_ecdsa_signature(std::move(ecdsa_signature));
      }

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
      ecdsa_signature_promise = std::promise<std::string>();
    }
  }
}
} // namespace complete_flow