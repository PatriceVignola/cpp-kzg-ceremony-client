#ifndef CONTRIBUTION_RESPONSE_H
#define CONTRIBUTION_RESPONSE_H

#include "contribution.h"
#include "hex_util.h"
#include "powers_of_tau.h"
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

#endif // CONTRIBUTION_RESPONSE_H