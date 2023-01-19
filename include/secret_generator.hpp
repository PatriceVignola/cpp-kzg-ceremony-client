#ifndef SECRET_GENERATOR_HPP
#define SECRET_GENERATOR_HPP

#include <array>
#include <cstdint>
#include <set>
#include <string>

#ifdef _DLL
#undef _DLL
#include <uint256_t.h>
#define _DLL
#else
#include <uint256_t.h>
#endif

class SecretGenerator {
public:
  SecretGenerator(const std::string& entropy_string, size_t num_secrets);
  const std::set<uint256_t>& get_secrets() const { return secrets_; }

private:
  // Since the number of secrets is expected to be low, use std::set instead of
  // std::unordered_set
  std::set<uint256_t> secrets_;
};

#endif // SECRET_GENERATOR_HPP