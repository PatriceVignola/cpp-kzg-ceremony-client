#ifndef AUTH_INFO_HPP
#define AUTH_INFO_HPP

#include <string>

class AuthInfo {
public:
  AuthInfo(std::string provider, std::string session_id, std::string nickname)
      : provider_(std::move(provider)), session_id_(std::move(session_id)),
        nickname_(std::move(nickname)) {}

  const std::string& get_provider() const { return provider_; }
  const std::string& get_session_id() const { return session_id_; }
  const std::string& get_nickname() const { return nickname_; }

private:
  const std::string provider_;
  const std::string session_id_;
  const std::string nickname_;
};

#endif // AUTH_INFO_HPP