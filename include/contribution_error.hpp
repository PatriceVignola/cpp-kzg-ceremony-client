#ifndef CONTRIBUTION_ERROR_HPP
#define CONTRIBUTION_ERROR_HPP

#include <absl/strings/string_view.h>
#include <nlohmann/json.hpp>
#include <string>

class ContributionError {
public:
  absl::string_view get_code() const { return code_; }
  absl::string_view get_error() const { return error_; }

private:
  std::string code_;
  std::string error_;

  friend void from_json(const nlohmann::json& json_contribution_error,
                        ContributionError& contribution_error);
};

#endif // CONTRIBUTION_ERROR_HPP