#ifndef ARG_PARSER_HPP
#define ARG_PARSER_HPP

#include <absl/strings/string_view.h>
#include <absl/types/optional.h>
#include <string>
#include <vector>

enum class AuthProvider {
  Ethereum,
  GitHub,
};

enum class EntropyType {
  Stdin,
  Drand,
  Https,
};

enum class ClientMode {
  Complete,
  Airgapped,
  Internet,
};

class ArgParser {
public:
  ArgParser(int argc, const char* const* argv);
  absl::string_view get_sequencer_url() const { return sequencer_url_; }
  AuthProvider get_auth_provider() const { return auth_provider_; }
  bool get_help_wanted() const { return help_wanted_; }
  absl::string_view get_help_message() const { return help_message_; };
  bool signing_disabled() const { return signing_disabled_; }
  std::vector<uint8_t> get_entropy() const;
  const absl::optional<uint16_t>& get_port() const { return port_; }
  ClientMode get_client_mode() const { return client_mode_; }
  absl::string_view get_session_id() const { return session_id_; }
  absl::string_view get_nickname() const { return nickname_; }

  const std::string& get_contribution_file_path() const {
    return contribution_file_path_;
  }

private:
  AuthProvider auth_provider_;
  std::string sequencer_url_;
  std::string help_message_;
  std::string contribution_file_path_;
  bool signing_disabled_;
  bool help_wanted_;
  EntropyType entropy_type_;
  absl::optional<uint16_t> port_;
  ClientMode client_mode_;
  std::string session_id_;
  std::string nickname_;
};

#endif // ARG_PARSER_HPP