#ifndef BATCH_TRANSCRIPT_HPP
#define BATCH_TRANSCRIPT_HPP

#include "include/transcript.hpp"
#include <nlohmann/json.hpp>
#include <vector>

class BatchTranscript {
public:
  BatchTranscript(const nlohmann::json& json_batch_transcript,
                  const nlohmann::json& json_schema);

  void validate_running_products() const;

  const std::vector<Transcript>& get_transcripts() const {
    return transcripts_;
  }

private:
  std::vector<Transcript> transcripts_;
  std::vector<std::string> participant_ids_;
  std::vector<std::string> participant_ecdsa_signatures_;
  std::string ecdsa_signature_;

  friend void to_json(nlohmann::json& json_batch_transcript,
                      const BatchTranscript& batch_transcript);
};

#endif // BATCH_TRANSCRIPT_HPP