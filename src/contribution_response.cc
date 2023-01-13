#include "include/contribution_response.h"
#include <nlohmann/json.hpp>
#include <sstream>
#include <valijson/adapters/nlohmann_json_adapter.hpp>
#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>
#include <valijson/validator.hpp>

using nlohmann::json;

void from_json(const json& json_contribution, Contribution& contribution) {
  json_contribution.at("numG1Powers").get_to(contribution.num_g1_powers_);
  json_contribution.at("numG2Powers").get_to(contribution.num_g2_powers_);
  json_contribution.at("powersOfTau").get_to(contribution.powers_of_tau_);
  json_contribution.at("potPubkey").get_to(contribution.pot_pubkey_);
  json_contribution.at("blsSignature").get_to(contribution.bls_signature_);
}

void from_json(const json& json_powers_of_tau, PowersOfTau& powers_of_tau) {
  auto g1_powers =
      json_powers_of_tau.at("G1Powers").get<std::vector<std::string>>();
  auto g2_powers =
      json_powers_of_tau.at("G2Powers").get<std::vector<std::string>>();

  powers_of_tau.g1_powers.reserve(g1_powers.size());
  std::transform(g1_powers.begin(), g1_powers.end(),
                 std::back_inserter(powers_of_tau.g1_powers),
                 [](const auto& power) { return G1Power(power); });

  powers_of_tau.g2_powers.reserve(g2_powers.size());
  std::transform(g2_powers.begin(), g2_powers.end(),
                 std::back_inserter(powers_of_tau.g2_powers),
                 [](const auto& power) { return G2Power(power); });
}

ContributionResponse::ContributionResponse(const json& json_response,
                                           const json& json_schema) {

  valijson::Schema schema;
  valijson::SchemaParser parser;
  valijson::adapters::NlohmannJsonAdapter schema_document_adapter(json_schema);
  parser.populateSchema(schema_document_adapter, schema);

  valijson::Validator validator(valijson::Validator::kStrongTypes);
  valijson::ValidationResults results;
  valijson::adapters::NlohmannJsonAdapter target_document_adapter(
      json_response);

  if (!validator.validate(schema, target_document_adapter, &results)) {
    std::cout << "Validation failed." << std::endl;
    valijson::ValidationResults::Error error;

    int error_num = 1;
    std::stringstream error_stream;

    while (results.popError(error)) {
      std::string context;
      for (const auto& error_string : error.context) {
        context += error_string;
      }

      error_stream << "Error #" << error_num << std::endl
                   << "  context: " << context << std::endl
                   << "  desc:    " << error.description << std::endl;
      ++error_num;
    }
    throw std::runtime_error(error_stream.str());
  }

  // If we reached this point, it means the JSON was correctly validated against
  // the schema and we can safely read fields and assume their concrete types
  contributions_ =
      json_response.at("contributions").get<std::vector<Contribution>>();

  ecdsa_signature_ = json_response.at("ecdsaSignature").get<std::string>();
}

void ContributionResponse::validate_powers() const {
  for (const auto& contribution : contributions_) {
    if (!contribution.get_powers_of_tau().valid()) {
      throw std::runtime_error("not all powers of Tau are elements of the "
                               "prime-ordered subgroup");
    }
  }
}