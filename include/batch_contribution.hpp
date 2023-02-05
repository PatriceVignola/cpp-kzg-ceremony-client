#ifndef BATCH_CONTRIBUTION_HPP
#define BATCH_CONTRIBUTION_HPP

#include "include/contribution.hpp"
#include <absl/types/span.h>
#include <nlohmann/json.hpp>
#include <vector>

class BatchContribution {
public:
  BatchContribution(const nlohmann::json& json_batch_contribution,
                    const nlohmann::json& json_schema);

  absl::Span<Contribution> get_contributions() {
    return absl::Span<Contribution>(contributions_);
  }

  void validate_powers() const;

  void set_ecdsa_signature(std::string&& ecdsa_signature) {
    ecdsa_signature_ = std::move(ecdsa_signature);
  }

private:
  std::vector<Contribution> contributions_;
  std::string ecdsa_signature_;

  friend void to_json(nlohmann::json& json_batch_contribution,
                      const BatchContribution& batch_contribution);
};

#endif // BATCH_CONTRIBUTION_HPP