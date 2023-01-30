#include "include/auth_browser.hpp"
#include <cpr/cpr.h>
#include <iostream>

#ifdef _WIN32
#include <shellapi.h>
#else
#include <csignal>
#include <unistd.h>
#endif

static constexpr auto info_message =
    "The authentication window should now open in your browser. If you close "
    "it by mistake or an error occurred during the authentication, you can "
    "manually access the following URL: \n\n";

#ifdef _WIN32
AuthBrowser::AuthBrowser(const std::string& auth_url) {
  std::cout << std::string(info_message) << auth_url << std::endl << std::endl;
  ShellExecuteA(0, 0, auth_url.c_str(), 0, 0, SW_SHOW);
}

AuthBrowser::~AuthBrowser() {}
#else
AuthBrowser::AuthBrowser(const std::string& auth_url) : pid_(fork()) {
  if (pid_ < 0) {
    throw std::runtime_error(
        "Failed to open the authentication page in the browser");
  }

  if (pid_ > 0) {
    std::cout << std::string(info_message) << auth_url << std::endl
              << std::endl;
  } else {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    execl("/usr/bin/xdg-open", "xdg-open", auth_url.c_str(), nullptr);
  }
}

AuthBrowser::~AuthBrowser() {
  if (pid_ > 0) {
    kill(pid_, SIGTERM);
  }
}
#endif
