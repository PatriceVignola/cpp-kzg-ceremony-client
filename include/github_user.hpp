#ifndef GITHUB_USER_HPP
#define GITHUB_USER_HPP

#include <absl/strings/string_view.h>
#include <nlohmann/json.hpp>
#include <string>

class GithubUser {
public:
  absl::string_view get_login() const { return login_; }
  int get_id() const { return id_; }

private:
  std::string login_;
  int id_;

  friend void from_json(const nlohmann::json& json_github_user,
                        GithubUser& github_user);
};

#endif // GITHUB_USER_HPP