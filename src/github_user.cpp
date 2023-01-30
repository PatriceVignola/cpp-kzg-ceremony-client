#include "include/github_user.hpp"

void from_json(const nlohmann::json& json_github_user,
               GithubUser& github_user) {
  json_github_user.at("login").get_to(github_user.login_);
  json_github_user.at("id").get_to(github_user.id_);
}