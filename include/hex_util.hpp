#ifndef HEX_UTIL_HPP
#define HEX_UTIL_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace hex_util {
std::string encode(const std::vector<uint8_t>& hex_bytes);
std::vector<uint8_t> decode(const std::string& hex);
} // namespace hex_util

#endif // HEX_UTIL_HPP