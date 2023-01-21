#include "include/contribution_error.hpp"

void from_json(const nlohmann::json& json_contribution_error,
               ContributionError& contribution_error) {
  json_contribution_error.at("code").get_to(contribution_error.code_);
  json_contribution_error.at("error").get_to(contribution_error.error_);
}