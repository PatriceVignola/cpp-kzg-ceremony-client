#ifndef BLS_SIGNER_HPP
#define BLS_SIGNER_HPP

#include <absl/strings/string_view.h>
#include <blst.hpp>
#include <string>

namespace bls_signer {
blst::P1_Affine sign(blst::Scalar secret, absl::string_view identity);
} // namespace bls_signer

#endif // BLS_SIGNER_HPP