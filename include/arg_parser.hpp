#ifndef ARG_PARSER_HPP
#define ARG_PARSER_HPP

#include <absl/strings/string_view.h>
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

class ArgParser {
public:
  ArgParser(int argc, const char* const* argv);
  absl::string_view get_sequencer_url() const { return sequencer_url_; }
  AuthProvider get_auth_provider() const { return auth_provider_; }
  bool get_help_wanted() const { return help_wanted_; }
  absl::string_view get_help_message() const { return help_message_; };
  bool signing_disabled() const { return signing_disabled_; }
  std::vector<uint8_t> get_entropy() const;

private:
  AuthProvider auth_provider_;
  std::string sequencer_url_;
  std::string help_message_;
  bool signing_disabled_;
  bool help_wanted_;
  EntropyType entropy_type_;
};

#endif // ARG_PARSER_HPP