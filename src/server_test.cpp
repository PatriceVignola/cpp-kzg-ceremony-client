#include "include/ascii_title.hpp"
#include "include/port_picker.hpp"
#include "include/server.hpp"
#include <absl/strings/str_cat.h>
#include <cpr/cpr.h>
#include <gtest/gtest.h>

// NOLINTNEXTLINE
TEST(TestServer, NotifiesWithErrorIfAlreadyContributed) {
  const auto port = port_picker::pick_unused_port();

  std::promise<AuthInfo> auth_info_promise;
  Server server(port,
                [&auth_info_promise](AuthInfo&& auth_info) {
                  auth_info_promise.set_value(std::move(auth_info));
                },
                nullptr, {});

  const auto url = absl::StrCat(
      "http://localhost:", port,
      "/auth_callback?code=AuthErrorPayload::UserAlreadyContributed");

  static constexpr int expected_status_code = 200;
  static constexpr auto expected_error =
      "You already tried contributing with this account. You can try "
      "contributing with another GitHub account or Ethereum address.";

  auto response = cpr::Get(cpr::Url{url});
  EXPECT_EQ(expected_status_code, response.status_code);
  EXPECT_EQ(absl::StrCat(ascii_title, "\n", expected_error, "\n"),
            response.text);

  auto auth_future = auth_info_promise.get_future();
  AuthInfo auth_info = auth_future.get();
  EXPECT_EQ(expected_error, auth_info.get_error_message());
}

// NOLINTNEXTLINE
TEST(TestServer, NotifiesWithErrorIfSessionIdNotFound) {
  const auto port = port_picker::pick_unused_port();

  std::promise<AuthInfo> auth_info_promise;
  Server server(port,
                [&auth_info_promise](AuthInfo&& auth_info) {
                  auth_info_promise.set_value(std::move(auth_info));
                },
                nullptr, {});

  const auto url = absl::StrCat("http://localhost:", port, "/auth_callback");

  static constexpr int expected_status_code = 200;
  static constexpr auto expected_error =
      "Failed to find `session_id` in the query arguments";

  auto response = cpr::Get(cpr::Url{url});
  EXPECT_EQ(expected_status_code, response.status_code);
  EXPECT_EQ(absl::StrCat(ascii_title, "\n", expected_error, "\n"),
            response.text);

  auto auth_future = auth_info_promise.get_future();
  AuthInfo auth_info = auth_future.get();
  EXPECT_EQ(expected_error, auth_info.get_error_message());
}

// NOLINTNEXTLINE
TEST(TestServer, NotifiesWithSessionInfoWhenSuccessful) {
  const auto port = port_picker::pick_unused_port();

  std::promise<AuthInfo> auth_info_promise;
  Server server(port,
                [&auth_info_promise](AuthInfo&& auth_info) {
                  auth_info_promise.set_value(std::move(auth_info));
                },
                nullptr, {});

  const auto url =
      absl::StrCat("http://localhost:", port,
                   "/auth_callback?session_id=my_session_id&nickname=my_"
                   "nickname&provider=my_provider");

  static constexpr int expected_status_code = 200;
  static const auto expected_message = absl::StrCat(
      ascii_title,
      "\nYou successfully authenticated with ID `my_nickname` and provider "
      "`my_provider`! You can now close this tab and go back to the "
      "application.\n");

  auto response = cpr::Get(cpr::Url{url});
  EXPECT_EQ(expected_status_code, response.status_code);
  EXPECT_EQ(expected_message, response.text);

  auto auth_future = auth_info_promise.get_future();
  AuthInfo auth_info = auth_future.get();
  EXPECT_EQ("my_session_id", auth_info.get_session_id());
  EXPECT_EQ("my_nickname", auth_info.get_nickname());
  EXPECT_EQ("my_provider", auth_info.get_provider());
}