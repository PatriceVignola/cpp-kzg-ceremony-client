#include "include/batch_contribution.hpp"
#include <iostream>
#include <valijson/adapters/nlohmann_json_adapter.hpp>
#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>
#include <valijson/validation_results.hpp>
#include <valijson/validator.hpp>

void from_json(const nlohmann::json& json_batch_contribution,
               BatchContribution& batch_contribution) {
  // If we reached this point, it means the JSON was correctly validated against
  // the schema and we can safely read fields and assume their concrete types
  json_batch_contribution.at("contributions")
      .get_to(batch_contribution.contributions_);
  json_batch_contribution.at("ecdsaSignature")
      .get_to(batch_contribution.ecdsa_signature_);
}

void to_json(nlohmann::json& json_batch_contribution,
             const BatchContribution& batch_contribution) {
  json_batch_contribution["contributions"] = batch_contribution.contributions_;
  json_batch_contribution["ecdsaSignature"] =
      batch_contribution.ecdsa_signature_;
}

BatchContribution::BatchContribution(const nlohmann::json& json_response,
                                     const nlohmann::json& json_schema) {
  // Validate the contribution file against the JSON schema
  std::cout << "Validating contribution file against the schema... ";

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

  std::cout << "Done!" << std::endl;
}

void BatchContribution::validate_powers() const {
  std::cout << "Validating powers of Tau... ";

  for (const auto& contribution : contributions_) {
    if (!contribution.get_powers_of_tau().valid()) {
      std::cout << "Validation failed." << std::endl;
      throw std::runtime_error("not all powers of Tau are elements of the "
                               "prime-ordered subgroup");
    }
  }

  std::cout << "Done!" << std::endl;
}