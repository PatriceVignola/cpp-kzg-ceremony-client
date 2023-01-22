#include "include/auth_callback_server.hpp"
#include "include/ascii_title.hpp"
#include "include/port_picker.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>

static constexpr int success_status_code = 200;

AuthCallbackServer::AuthCallbackServer(
    uint16_t port, const std::function<void(AuthInfo)>& on_auth_received) {
  service_thread_ = std::thread([this, port, on_auth_received]() {
    auto resource = std::make_shared<restbed::Resource>();
    resource->set_path("/auth_callback");
    resource->set_method_handler("GET", [this, on_auth_received](
                                            const std::shared_ptr<
                                                restbed::Session>& session) {
      const auto request = session->get_request();

      size_t content_length = request->get_header("Content-Length", 0);

      session->fetch(content_length, [this, request, on_auth_received](
                                         const std::shared_ptr<
                                             restbed::Session>& session,
                                         const restbed::Bytes& /*body*/) {
        auto code = request->get_query_parameter("code");

        std::stringstream html_message;
        html_message << ascii_title << std::endl;

        if (code == "AuthErrorPayload::UserAlreadyContributed") {
          std::string error_message =
              "You already tried contributing with this account. You can try "
              "contributing with another GitHub account or Ethereum address.";
          html_message << error_message << std::endl;

          on_auth_received(AuthInfo(std::move(error_message)));
        } else {
          auto session_id = request->get_query_parameter("session_id");
          auto nickname = request->get_query_parameter("nickname");
          auto provider = request->get_query_parameter("provider");

          html_message
              << "You successfully authenticated with ID `" << nickname
              << "` and provider `" << provider
              << "`! You can now close this tab and go back to the application."
              << std::endl;

          on_auth_received(AuthInfo(std::move(provider), std::move(session_id),
                                    std::move(nickname)));
        }

        session->close(success_status_code, html_message.str());
      });
    });

    service_.publish(resource);
    service_.set_ready_handler([port](restbed::Service&) {
      std::cout << "Authentication server is listening on port " << port
                << std::endl;
    });
    auto settings = std::make_shared<restbed::Settings>();
    settings->set_port(port);
    service_.start(settings);
  });
}

AuthCallbackServer::~AuthCallbackServer() {
  service_.stop();
  service_thread_.join();
}