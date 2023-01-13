#ifndef HEX_UTIL_H
#define HEX_UTIL_H

#include <cstdint>
#include <string>
#include <vector>

namespace hex_util {
std::vector<uint8_t> decode(const std::string& hex);
} // namespace hex_util

#endif // HEX_UTIL_H