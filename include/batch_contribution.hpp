#ifndef BATCH_CONTRIBUTION_HPP
#define BATCH_CONTRIBUTION_HPP

#include "include/contribution.hpp"
#include <nlohmann/json.hpp>
#include <vector>

class BatchContribution {
public:
  BatchContribution(const nlohmann::json& json_batch_contribution,
                    const nlohmann::json& json_schema);

  std::vector<Contribution>& get_contributions() { return contributions_; }
  void validate_powers() const;

  void set_ecdsa_signature(std::string ecdsa_signature) {
    ecdsa_signature_ = std::move(ecdsa_signature);
  }

private:
  std::vector<Contribution> contributions_;
  std::string ecdsa_signature_;

  friend void to_json(nlohmann::json& json_batch_contribution,
                      const BatchContribution& batch_contribution);
};

#endif // BATCH_CONTRIBUTION_HPP