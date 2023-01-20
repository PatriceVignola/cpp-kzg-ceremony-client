#ifndef AUTH_INFO_HPP
#define AUTH_INFO_HPP

#include <string>

class AuthInfo {
public:
  AuthInfo(std::string provider, std::string session_id, std::string nickname)
      : provider_(std::move(provider)), session_id_(std::move(session_id)),
        nickname_(std::move(nickname)) {}

  explicit AuthInfo(std::string error_message)
      : error_message_(std::move(error_message)) {}

  const std::string& get_provider() const { return provider_; }
  const std::string& get_session_id() const { return session_id_; }
  const std::string& get_nickname() const { return nickname_; }
  const std::string& get_error_message() const { return error_message_; }

private:
  const std::string provider_;
  const std::string session_id_;
  const std::string nickname_;
  const std::string error_message_;
};

#endif // AUTH_INFO_HPP