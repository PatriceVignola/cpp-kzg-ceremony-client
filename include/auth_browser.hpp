#ifndef AUTH_BROWSER_HPP
#define AUTH_BROWSER_HPP

#include <string>

class AuthBrowser {
public:
  explicit AuthBrowser(const std::string& auth_url);
  AuthBrowser(const AuthBrowser& other) = delete;
  AuthBrowser(AuthBrowser&& other) = delete;
  AuthBrowser& operator=(const AuthBrowser& other) = delete;
  AuthBrowser& operator=(AuthBrowser&& other) = delete;
  ~AuthBrowser();

private:
#ifndef _WIN32
  int pid_;
#endif
};

#endif // AUTH_BROWSER_HPP