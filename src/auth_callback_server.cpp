#include "include/auth_callback_server.hpp"
#include "include/ascii_title.hpp"
#include "include/port_picker.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>

static constexpr int success_status_code = 200;

AuthCallbackServer::AuthCallbackServer(
    uint16_t port, const std::function<void(AuthInfo)>& on_auth_received) {

  std::promise<bool> server_started_promise;

  service_thread_ = std::thread([this, port, &on_auth_received,
                                 &server_started_promise]() {
    auto resource = std::make_shared<restbed::Resource>();
    resource->set_path("/auth_callback");
    resource->set_method_handler("GET", [this, on_auth_received](
                                            const std::shared_ptr<
                                                restbed::Session>& session) {
      const auto request = session->get_request();

      size_t content_length = request->get_header("Content-Length", 0);

      session->fetch(content_length, [this, request, &on_auth_received](
                                         const std::shared_ptr<
                                             restbed::Session>& session,
                                         const restbed::Bytes& /*body*/) {
        auto code = request->get_query_parameter("code");

        std::string html_message = std::string(ascii_title) + "\n";

        if (code == "AuthErrorPayload::UserAlreadyContributed") {
          std::string error_message =
              "You already tried contributing with this account. You can try "
              "contributing with another GitHub account or Ethereum address.";
          html_message += error_message + "\n";
          on_auth_received(AuthInfo(std::move(error_message)));
        } else if (!request->has_query_parameter("session_id")) {
          std::string error_message =
              "Failed to find `session_id` in the query arguments";
          html_message += error_message + "\n";
          on_auth_received(AuthInfo(std::move(error_message)));
        } else {
          auto session_id = request->get_query_parameter("session_id");
          auto nickname = request->get_query_parameter("nickname");
          auto provider = request->get_query_parameter("provider");

          html_message +=
              +"You successfully authenticated with ID `" + nickname +
              "` and provider `" + provider +
              "`! You can now close this tab and go back to the application.\n";

          on_auth_received(AuthInfo(std::move(provider), std::move(session_id),
                                    std::move(nickname)));
        }

        session->close(success_status_code, html_message);
      });
    });

    service_.publish(resource);
    service_.set_ready_handler(
        [port, &server_started_promise](restbed::Service& /*service*/) {
          std::cout << "Authentication server is listening on port " << port
                    << std::endl;
          server_started_promise.set_value(true);
        });

    service_.set_error_handler(
        [&server_started_promise](
            int /*status*/, const std::exception& error,
            const std::shared_ptr<restbed::Session>& /*session*/) {
          std::cout << "Authentication server error: " << error.what()
                    << std::endl;
          server_started_promise.set_value(false);
        });

    auto settings = std::make_shared<restbed::Settings>();
    settings->set_port(port);
    service_.start(settings);
  });

  auto server_started = server_started_promise.get_future().get();

  if (!server_started) {
    throw std::runtime_error("Failed to start the authentication server.");
  }
}

AuthCallbackServer::~AuthCallbackServer() {
  service_.stop();
  service_thread_.join();
}