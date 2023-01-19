#ifndef ARG_PARSER_HPP
#define ARG_PARSER_HPP

#include <string>

enum class AuthProvider {
  Ethereum,
  GitHub,
};

class ArgParser {
public:
  ArgParser(int argc, const char* const* argv);
  const std::string& get_sequencer_url() const { return sequencer_url_; }
  uint16_t get_auth_callback_port() const { return port_; }
  AuthProvider get_auth_provider() const { return auth_provider_; }
  bool get_help_wanted() const { return help_wanted_; }
  const std::string& get_help_message() const { return help_message_; };
  const std::string& get_entropy() const { return entropy_; }

private:
  AuthProvider auth_provider_;
  std::string sequencer_url_;
  std::string help_message_;
  std::string entropy_;
  uint16_t port_;
  bool help_wanted_;
};

#endif // ARG_PARSER_HPP