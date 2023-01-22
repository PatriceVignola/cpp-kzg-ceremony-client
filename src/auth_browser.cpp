#include "include/auth_browser.hpp"
#include <cpr/cpr.h>
#include <iostream>

#ifdef _WIN32
#include <shellapi.h>
#else
#include <csignal>
#include <unistd.h>
#endif

AuthBrowser::AuthBrowser(const std::string& auth_url) {
  std::cout << "Your browser should now open a tab with the authentication "
               "link. If you close the tab by mistake or an error occurred "
               "during the authentication, you can manually open the following "
               "URL in your browser: "
            << std::endl
            << std::endl
            << auth_url << std::endl
            << std::endl;

#ifdef _WIN32
  ShellExecuteA(0, 0, auth_url.c_str(), 0, 0, SW_SHOW);
#else
  pid_ = fork();
  if (pid_ == 0) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    execl("/usr/bin/xdg-open", "xdg-open", auth_url.c_str(), nullptr);
  } else if (pid_ < 0) {
    throw std::runtime_error("Failed to open authentication URL in browser");
  }
#endif
}

AuthBrowser::~AuthBrowser() {
#ifndef _WIN32
  if (pid_ > 0) {
    kill(pid_, SIGTERM);
  }
#endif
}