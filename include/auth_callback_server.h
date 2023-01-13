#ifndef AUTH_CALLBACK_SERVER_H
#define AUTH_CALLBACK_SERVER_H

#include "auth_info.h"
#include <cstdint>
#include <restbed>

class AuthCallbackServer {
public:
  explicit AuthCallbackServer(
      uint16_t port, const std::function<void(AuthInfo)>& on_auth_received);
  AuthCallbackServer(const AuthCallbackServer&) = delete;
  AuthCallbackServer(AuthCallbackServer&&) = delete;
  AuthCallbackServer& operator=(const AuthCallbackServer&) = delete;
  AuthCallbackServer& operator=(AuthCallbackServer&&) = delete;
  ~AuthCallbackServer();

private:
  restbed::Service service_;
  std::thread service_thread_;
};

#endif // AUTH_CALLBACK_SERVER_H
