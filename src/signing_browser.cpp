#include "include/signing_browser.hpp"
#include <absl/strings/string_view.h>
#include <cpr/cpr.h>
#include <iostream>

#ifdef _WIN32
#include <shellapi.h>
#else
#include <csignal>
#include <unistd.h>
#endif

static constexpr absl::string_view info_message =
    "The signing window should now open in your browser. If you close it by "
    "mistake or an error occurred during signing, you can manually access the "
    "following URL: \n\n";

#ifdef _WIN32
SigningBrowser::SigningBrowser(const std::string& auth_url) {
  std::cout << info_message << auth_url << std::endl << std::endl;
  ShellExecuteA(0, 0, auth_url.c_str(), 0, 0, SW_SHOW);
}

SigningBrowser::~SigningBrowser() {}
#else
SigningBrowser::SigningBrowser(const std::string& auth_url) : pid_(fork()) {
  if (pid_ < 0) {
    throw std::runtime_error("Failed to open the signing page in the browser");
  }

  if (pid_ > 0) {
    std::cout << info_message << auth_url << std::endl << std::endl;
  } else {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    execl("/usr/bin/xdg-open", "xdg-open", auth_url.c_str(), nullptr);
  }
}

SigningBrowser::~SigningBrowser() {
  if (pid_ > 0) {
    kill(pid_, SIGTERM);
  }
}
#endif
