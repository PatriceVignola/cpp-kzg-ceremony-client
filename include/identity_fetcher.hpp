#ifndef IDENTITY_FETCHER_HPP
#define IDENTITY_FETCHER_HPP

#include <absl/strings/string_view.h>
#include <string>

namespace identity_fetcher {
std::string get_ethereum_identity(absl::string_view ethereum_address);
std::string get_github_identity(absl::string_view github_nickname);
} // namespace identity_fetcher

#endif // IDENTITY_FETCHER_HPP