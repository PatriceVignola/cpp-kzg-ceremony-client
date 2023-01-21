#ifndef BATCH_CONTRIBUTION_HPP
#define BATCH_CONTRIBUTION_HPP

#include "include/contribution.hpp"
#include <nlohmann/json.hpp>
#include <vector>

class BatchContribution {
public:
  BatchContribution(const nlohmann::json& json_response,
                    const nlohmann::json& json_schema);

  std::vector<Contribution>& get_contributions() { return contributions_; }
  void validate_powers() const;

private:
  std::vector<Contribution> contributions_;
  std::string ecdsa_signature_;

  friend void from_json(const nlohmann::json& json_batch_contribution,
                        BatchContribution& batch_contribution);

  friend void to_json(nlohmann::json& json_batch_contribution,
                      const BatchContribution& batch_contribution);
};

#endif // BATCH_CONTRIBUTION_HPP