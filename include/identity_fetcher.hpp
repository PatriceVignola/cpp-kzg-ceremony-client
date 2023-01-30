#ifndef IDENTITY_FETCHER_HPP
#define IDENTITY_FETCHER_HPP

#include <string>

namespace identity_fetcher {
std::string get_ethereum_identity(std::string ethereum_address);
std::string get_github_identity(const std::string& github_nickname);
} // namespace identity_fetcher

#endif // IDENTITY_FETCHER_HPP