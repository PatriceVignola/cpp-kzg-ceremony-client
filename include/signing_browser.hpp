#ifndef SIGNING_BROWSER_HPP
#define SIGNING_BROWSER_HPP

#include <string>

class SigningBrowser {
public:
  explicit SigningBrowser(const std::string& auth_url);
  SigningBrowser(const SigningBrowser& other) = delete;
  SigningBrowser(SigningBrowser&& other) = delete;
  SigningBrowser& operator=(const SigningBrowser& other) = delete;
  SigningBrowser& operator=(SigningBrowser&& other) = delete;
  ~SigningBrowser();

private:
#ifndef _WIN32
  const int pid_;
#endif
};

#endif // SIGNING_BROWSER_HPP