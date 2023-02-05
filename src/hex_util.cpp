#include "include/hex_util.hpp"
#include <absl/strings/numbers.h>
#include <absl/strings/string_view.h>
#include <absl/types/span.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace hex_util {
std::string encode(absl::Span<const uint8_t> hex_bytes) {
  std::stringstream encoded_hex_stream;
  encoded_hex_stream << "0x" << std::hex;

  for (uint8_t byte : hex_bytes) {
    encoded_hex_stream << std::setw(2) << std::setfill('0')
                       << static_cast<int>(byte);
  }

  return encoded_hex_stream.str();
}

std::vector<uint8_t> decode(absl::string_view hex) {
  std::vector<uint8_t> bytes;
  bytes.reserve((hex.size() - 2) / 2);

  for (size_t i = 2; i < hex.length(); i += 2) {
    int byte = 0;
    absl::SimpleHexAtoi(hex.substr(i, 2), &byte);
    bytes.push_back(static_cast<uint8_t>(byte));
  }

  return bytes;
}
} // namespace hex_util