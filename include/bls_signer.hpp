#ifndef BLS_SIGNER_HPP
#define BLS_SIGNER_HPP

#include <blst.hpp>
#include <string>

#ifdef _DLL
#undef _DLL
#include <uint256_t.h>
#define _DLL
#else
#include <uint256_t.h>
#endif

namespace bls_signer {
blst::P1_Affine sign(uint256_t secret, const std::string& identity);
} // namespace bls_signer

#endif // BLS_SIGNER_HPP