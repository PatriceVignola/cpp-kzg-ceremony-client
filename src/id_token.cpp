#include "include/id_token.hpp"

void from_json(const nlohmann::json& json_id_token, IdToken& id_token) {
  json_id_token.at("sub").get_to(id_token.sub_);
  json_id_token.at("nickname").get_to(id_token.nickname_);
  json_id_token.at("provider").get_to(id_token.provider_);
  json_id_token.at("exp").get_to(id_token.exp_);
}