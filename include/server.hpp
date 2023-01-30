#ifndef SERVER_HPP
#define SERVER_HPP

#include "auth_info.hpp"
#include "include/pot_pubkey_message.hpp"
#include <cstdint>

#define WIN_DLL_EXPORT
#include <restbed>
#undef WIN_DLL_EXPORT

class BatchTranscript;

class Server {
public:
  explicit Server(uint16_t port,
                  const std::function<void(AuthInfo)>& on_auth_received,
                  const std::function<void(std::string)>& on_signature_received,
                  const std::vector<PotPubkeyMessage>& pot_pubkey_message);
  Server(const Server&) = delete;
  Server(Server&&) = delete;
  Server& operator=(const Server&) = delete;
  Server& operator=(Server&&) = delete;
  ~Server();

private:
  restbed::Service service_;
  std::thread service_thread_;
};

#endif // SERVER_HPP
