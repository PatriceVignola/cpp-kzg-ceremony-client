#include "include/hex_util.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace hex_util {
std::string encode(const std::vector<uint8_t>& hex_bytes) {
  std::stringstream encoded_hex_stream;
  encoded_hex_stream << "0x" << std::hex;

  for (uint8_t byte : hex_bytes) {
    encoded_hex_stream << std::setw(2) << std::setfill('0')
                       << static_cast<int>(byte);
  }

  return encoded_hex_stream.str();
}

std::vector<uint8_t> decode(const std::string& hex) {
  std::cout << "43" << std::endl;
  std::vector<uint8_t> bytes;
  std::cout << "44" << std::endl;
  bytes.reserve((hex.size() - 2) / 2);
  std::cout << "45" << std::endl;

  for (size_t i = 2; i < hex.length(); i += 2) {
    std::cout << i << ": 46" << std::endl;
    constexpr static int hex_base = 16;
    std::cout << i << ": 47" << std::endl;
    uint8_t byte = std::stoul(hex.substr(i, 2), nullptr, hex_base);
    std::cout << i << ": 48" << std::endl;
    bytes.push_back(byte);
    std::cout << i << ": 49" << std::endl;
  }

  std::cout << "50" << std::endl;

  return bytes;
}
} // namespace hex_util