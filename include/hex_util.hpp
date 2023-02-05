#ifndef HEX_UTIL_HPP
#define HEX_UTIL_HPP

#include <absl/strings/string_view.h>
#include <absl/types/span.h>
#include <cstdint>
#include <string>
#include <vector>

namespace hex_util {
std::string encode(absl::Span<const uint8_t> hex_bytes);
std::vector<uint8_t> decode(absl::string_view hex);
} // namespace hex_util

#endif // HEX_UTIL_HPP