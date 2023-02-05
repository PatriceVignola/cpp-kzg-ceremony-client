#include "include/batch_transcript.hpp"
#include <absl/strings/str_cat.h>
#include <iostream>
#include <valijson/adapters/nlohmann_json_adapter.hpp>
#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>
#include <valijson/validation_results.hpp>
#include <valijson/validator.hpp>

void to_json(nlohmann::json& json_batch_transcript,
             const BatchTranscript& batch_transcript) {
  json_batch_transcript["transcripts"] = batch_transcript.transcripts_;
  json_batch_transcript["participantIds"] = batch_transcript.participant_ids_;
  json_batch_transcript["participantEcdsaSignatures"] =
      batch_transcript.participant_ecdsa_signatures_;
}

BatchTranscript::BatchTranscript(const nlohmann::json& json_batch_transcript,
                                 const nlohmann::json& json_schema) {
  // Validate the transcript file against the JSON schema
  std::cout << "Validating transcript file against the schema" << std::endl;

  valijson::Schema schema;
  valijson::SchemaParser parser;
  valijson::adapters::NlohmannJsonAdapter schema_document_adapter(json_schema);
  parser.populateSchema(schema_document_adapter, schema);

  valijson::Validator validator(valijson::Validator::kStrongTypes);
  valijson::ValidationResults results;
  valijson::adapters::NlohmannJsonAdapter target_document_adapter(
      json_batch_transcript);

  if (!validator.validate(schema, target_document_adapter, &results)) {
    std::cout << "Validation failed." << std::endl;
    valijson::ValidationResults::Error error;

    int error_num = 1;
    std::string error_message;

    while (results.popError(error)) {
      absl::StrAppend(&error_message, "Error #", error_num, "\n  context: ");

      for (const auto& error_string : error.context) {
        absl::StrAppend(&error_message, error_string);
      }

      absl::StrAppend(&error_message, "\n  desc:    ", error.description, "\n");
      ++error_num;
    }
    throw std::runtime_error(error_message);
  }

  // If we reached this point, it means the JSON was correctly validated against
  // the schema and we can safely read fields and assume their concrete types
  json_batch_transcript.at("transcripts").get_to(transcripts_);
  json_batch_transcript.at("participantIds").get_to(participant_ids_);
  json_batch_transcript.at("participantEcdsaSignatures")
      .get_to(participant_ecdsa_signatures_);
}

void BatchTranscript::validate_running_products() const {
  std::cout << "Validating the transcript's running products" << std::endl;

  for (const auto& transcript : transcripts_) {
    if (!transcript.get_witness().valid()) {
      std::cout << "Validation failed." << std::endl;
      throw std::runtime_error("not all running products are elements of the "
                               "prime-ordered subgroup");
    }
  }
}