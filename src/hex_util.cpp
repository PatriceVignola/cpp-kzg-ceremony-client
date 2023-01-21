#include "include/hex_util.hpp"
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace hex_util {
std::string encode(const std::vector<uint8_t>& hex_bytes) {
  std::string encoded_hex;
  encoded_hex.reserve(hex_bytes.size() * 2 + 2);

  std::stringstream stream;

  encoded_hex += "0x";

  for (uint8_t byte : hex_bytes) {
    constexpr static int hex_base = 16;
    stream << "0x" << std::setfill('0') << std::setw(sizeof(byte) * 2)
           << std::hex << byte;
  }

  return encoded_hex;
}

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