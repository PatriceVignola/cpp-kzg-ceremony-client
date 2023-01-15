#ifndef CONTRIBUTION_RESPONSE_HPP
#define CONTRIBUTION_RESPONSE_HPP

#include "contribution.hpp"
#include "hex_util.hpp"
#include "powers_of_tau.hpp"
#include <blst.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class ContributionResponse {
public:
  explicit ContributionResponse(const nlohmann::json& json_response,
                                const nlohmann::json& schema);

  const std::vector<Contribution>& get_contributions() const {
    return contributions_;
  }

  void validate_powers() const;

private:
  std::vector<Contribution> contributions_;
  std::string ecdsa_signature_;
};

#endif // CONTRIBUTION_RESPONSE_HPP