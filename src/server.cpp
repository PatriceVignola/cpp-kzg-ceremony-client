#include "include/server.hpp"
#include "include/ascii_title.hpp"
#include "include/batch_transcript.hpp"
#include "include/ethereum_signing_html.hpp"
#include "include/port_picker.hpp"
#include "include/pot_pubkey_message.hpp"
#include <absl/strings/str_cat.h>
#include <iostream>
#include <nlohmann/json.hpp>

static constexpr int success_status_code = 200;

static std::shared_ptr<restbed::Resource> make_auth_resource(
    const std::function<void(AuthInfo)>& on_auth_received) {
  auto resource = std::make_shared<restbed::Resource>();
  resource->set_path("/auth_callback");
  resource->set_method_handler("GET", [on_auth_received](
                                          const std::shared_ptr<
                                              restbed::Session>& session) {
    const auto request = session->get_request();

    size_t content_length = request->get_header("Content-Length", 0);

    session->fetch(content_length, [request, &on_auth_received](
                                       const std::shared_ptr<restbed::Session>&
                                           session,
                                       const restbed::Bytes& /*body*/) {
      auto code = request->get_query_parameter("code");

      std::string html_message = absl::StrCat(ascii_title, "\n");

      if (code == "AuthErrorPayload::UserAlreadyContributed") {
        std::string error_message =
            "You already tried contributing with this account. You can try "
            "contributing with another GitHub account or Ethereum address.";
        absl::StrAppend(&html_message, error_message, "\n");
        on_auth_received(AuthInfo(std::move(error_message)));
      } else if (!request->has_query_parameter("session_id")) {
        std::string error_message =
            "Failed to find `session_id` in the query arguments";
        absl::StrAppend(&html_message, error_message, "\n");
        on_auth_received(AuthInfo(std::move(error_message)));
      } else {
        auto session_id = request->get_query_parameter("session_id");
        auto nickname = request->get_query_parameter("nickname");
        auto provider = request->get_query_parameter("provider");

        absl::StrAppend(
            &html_message, +"You successfully authenticated with ID `",
            nickname, "` and provider `", provider,
            "`! You can now close this tab and go back to the application.\n");
        on_auth_received(AuthInfo(std::move(provider), std::move(session_id),
                                  std::move(nickname)));
      }

      session->close(success_status_code, html_message);
    });
  });

  return resource;
}

static std::shared_ptr<restbed::Resource> make_signing_resource(
    uint16_t port, absl::Span<const PotPubkeyMessage> pot_pubkey_messages) {
  auto resource = std::make_shared<restbed::Resource>();
  resource->set_path("/sign");
  resource->set_method_handler(
      "GET", [port, &pot_pubkey_messages](
                 const std::shared_ptr<restbed::Session>& session) {
        auto signing_objects_json = R"(
          {
            "types": {
              "EIP712Domain": [
                {
                  "name": "name",
                  "type": "string"
                },
                {
                  "name": "version",
                  "type": "string"
                },
                {
                  "name": "chainId",
                  "type": "uint256"
                }
              ],
              "contributionPubkey": [
                {
                  "name": "numG1Powers",
                  "type": "uint256"
                },
                {
                  "name": "numG2Powers",
                  "type": "uint256"
                },
                {
                  "name": "potPubkey",
                  "type": "bytes"
                }
              ],
              "PoTPubkeys": [
                {
                  "name": "potPubkeys",
                  "type": "contributionPubkey[]"
                }
              ]
            },
            "primaryType": "PoTPubkeys",
            "domain": {
              "name": "Ethereum KZG Ceremony",
              "version": "1.0",
              "chainId": 1
            },
            "message": {
              "potPubkeys": []
            }
          }
        )"_json;

        signing_objects_json["message"]["potPubkeys"] = pot_pubkey_messages;

        const auto serialized_json = signing_objects_json.dump();
        const auto request = session->get_request();
        size_t content_length = request->get_header("Content-Length", 0);

        session->fetch(
            content_length,
            [request, port,
             &serialized_json](const std::shared_ptr<restbed::Session>& session,
                               const restbed::Bytes& /*body*/) {
              auto eth_address = request->get_query_parameter("eth_address");

              std::string html_message = ethereum_signing_html;

              // Inject the banner
              html_message.replace(html_message.find("${banner}"),
                                   sizeof("${banner}") - 1, ascii_title);

              // Inject the Ethereum address
              html_message.replace(html_message.find("${ethAddress}"),
                                   sizeof("${ethAddress}") - 1, eth_address);

              // Inject the callback path
              const auto signing_callback =
                  absl::StrCat("http://localhost:", port, "/signing_callback");

              html_message.replace(html_message.find("${callbackPath}"),
                                   sizeof("${callbackPath}") - 1,
                                   signing_callback);

              // Inject the json object
              html_message.replace(html_message.find("${jsonData}"),
                                   sizeof("${jsonData}") - 1, serialized_json);

              session->close(success_status_code, html_message);
            });
      });

  return resource;
}

static std::shared_ptr<restbed::Resource> make_signing_callback_resource(
    const std::function<void(std::string)>& on_signature_received) {
  auto resource = std::make_shared<restbed::Resource>();
  resource->set_path("/signing_callback");
  resource->set_method_handler(
      "GET", [on_signature_received](
                 const std::shared_ptr<restbed::Session>& session) {
        const auto request = session->get_request();

        const auto fetch_callback =
            [request, &on_signature_received](
                const std::shared_ptr<restbed::Session>& session,
                const restbed::Bytes& /*body*/) {
              auto signature = request->get_query_parameter("signature");

              on_signature_received(signature);

              std::string html_message =
                  absl::StrCat("The contribution was successfully signed with "
                               "the following signature: ",
                               signature);

              session->close(success_status_code, html_message);
            };

        size_t content_length = request->get_header("Content-Length", 0);
        session->fetch(content_length, fetch_callback);
      });

  return resource;
}

Server::Server(uint16_t port,
               const std::function<void(AuthInfo)>& on_auth_received,
               const std::function<void(std::string)>& on_signature_received,
               absl::Span<const PotPubkeyMessage> pot_pubkey_messages) {

  std::promise<bool> server_started_promise;

  service_thread_ =
      std::thread([this, port, &on_auth_received, &on_signature_received,
                   &server_started_promise, &pot_pubkey_messages]() {
        auto auth_resource = make_auth_resource(on_auth_received);
        service_.publish(auth_resource);

        if (on_signature_received) {
          auto signing_resource =
              make_signing_resource(port, pot_pubkey_messages);
          service_.publish(signing_resource);

          auto signing_callback_resource =
              make_signing_callback_resource(on_signature_received);
          service_.publish(signing_callback_resource);
        }

        service_.set_ready_handler(
            [port, &server_started_promise](restbed::Service& /*service*/) {
              std::cout << "Server is listening on port " << port << std::endl;
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

Server::~Server() {
  service_.stop();
  service_thread_.join();
}