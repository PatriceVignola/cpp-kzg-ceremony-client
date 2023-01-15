#ifndef HEX_UTIL_HPP
#define HEX_UTIL_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace hex_util {
std::vector<uint8_t> decode(const std::string& hex);
} // namespace hex_util

#endif // HEX_UTIL_HPP