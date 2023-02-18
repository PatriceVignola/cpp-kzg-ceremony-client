#include "include/arg_parser.hpp"
#include "include/ascii_title.hpp"
#include "include/auth_browser.hpp"
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

namespace airgapped_flow {
void launch(const ArgParser& arg_parser) {
  // TODO: Read the contribution file from disk
  // TODO: Validate the contribution powers
  // TODO: Parse the identity from the json file and remove it from the json

  // Generate one secret for each contribution
  std::cout << "Generating secrets" << std::endl;
  static constexpr size_t num_secrets = 4;
  SecretGenerator<> secret_generator(arg_parser.get_entropy(), num_secrets);

  const auto& auth_provider = arg_parser.get_auth_provider();

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

  std::string ecdsa_signature;
  if (auth_provider == AuthProvider::Ethereum &&
      !arg_parser.signing_disabled() && ecdsa_signature.empty()) {
    const auto signing_url =
        absl::StrCat("http://localhost:", port,
                     "/sign?eth_address=", auth_info.get_nickname());

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

  // TODO: Save the updated contribution file
}
} // namespace airgapped_flow
