#ifndef AUTH_HELPERS_HPP
#define AUTH_HELPERS_HPP

#include "include/arg_parser.hpp"
#include "include/auth_info.hpp"
#include "sequencer_client.hpp"
#include <future>

class SequencerClient;

namespace auth_helpers {
AuthInfo authenticate(const SequencerClient& sequencer_client,
                      AuthProvider auth_provider,
                      std::promise<AuthInfo>& auth_info_promise);
} // namespace auth_helpers

#endif // AUTH_HELPERS_HPP