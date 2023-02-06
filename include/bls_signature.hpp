#ifndef BLS_SIGNATURE_HPP
#define BLS_SIGNATURE_HPP

#include <absl/strings/string_view.h>
#include <blst.hpp>
#include <string>

class BlsSignature {
public:
  BlsSignature(blst::Scalar secret, absl::string_view identity);
  std::string encode() const;

private:
  blst::P1_Affine bls_signature_;
};

#endif // BLS_SIGNATURE_HPP