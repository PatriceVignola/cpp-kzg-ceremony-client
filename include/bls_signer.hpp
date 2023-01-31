#ifndef BLS_SIGNER_HPP
#define BLS_SIGNER_HPP

#include <blst.hpp>
#include <string>

namespace bls_signer {
blst::P1_Affine sign(blst::Scalar secret, const std::string& identity);
} // namespace bls_signer

#endif // BLS_SIGNER_HPP