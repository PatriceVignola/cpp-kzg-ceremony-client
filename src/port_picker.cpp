#include "include/port_picker.hpp"
#include <cstdint>

#ifndef _WIN32
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <iostream>

namespace port_picker {

uint16_t pick_unused_port() {
  // Return port 8080 by default if anything bad happens
  static constexpr uint16_t default_port = 8080;

  // Find an available port
#ifdef _WIN32
  uint16_t port = default_port;
#else
  int sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    return default_port;
  }

  const int reuse_address = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse_address, sizeof(int)) <
      0) {
    return default_port;
  }

  sockaddr_in socket_address{};
  socket_address.sin_family = AF_INET;
  socket_address.sin_port = 0;
  socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
  while (bind(sock, static_cast<sockaddr*>(static_cast<void*>(&socket_address)),
              sizeof(socket_address)) < 0) {
  }

  socket_address = {};
  socklen_t len = sizeof(socket_address);
  getsockname(sock, static_cast<sockaddr*>(static_cast<void*>(&socket_address)),
              &len);

  uint16_t port = ntohs(socket_address.sin_port);

  // TODO (PatriceVignola): Use an RAII wrapper once we start using abseil
  close(sock);
#endif

  return port;
}

} // namespace port_picker