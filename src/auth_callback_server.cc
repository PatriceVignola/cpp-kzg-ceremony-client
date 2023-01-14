#include "include/auth_callback_server.h"
#include "include/ascii_title.h"
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
        auto session_id = request->get_query_parameter("session_id");
        auto nickname = request->get_query_parameter("nickname");
        auto provider = request->get_query_parameter("provider");

        std::stringstream success_message;
        success_message << ascii_title << std::endl;

        success_message
            << "You successfully authenticated with ID `" << nickname
            << "` and provider `" << provider
            << "`! You can now close this tab and go back to the application."
            << std::endl;

        on_auth_received(AuthInfo(std::move(provider), std::move(session_id),
                                  std::move(nickname)));

        session->close(success_status_code, success_message.str());
      });
    });

    service_.publish(resource);
    auto settings = std::make_shared<restbed::Settings>();
    settings->set_port(port);
    service_.start(settings);
  });
}

AuthCallbackServer::~AuthCallbackServer() {
  service_.stop();
  service_thread_.join();
}