#include "include/hex_util.hpp"
#include <string>
#include <vector>

namespace hex_util {
std::vector<uint8_t> decode(const std::string& hex) {
  std::vector<uint8_t> bytes;
  bytes.reserve((hex.size() - 2) / 2);

  for (size_t i = 2; i < hex.length(); i += 2) {
    constexpr static int hex_base = 16;
    uint8_t byte = std::stoul(hex.substr(i, 2), nullptr, hex_base);
    bytes.push_back(byte);
  }

  return bytes;
}
} // namespace hex_util