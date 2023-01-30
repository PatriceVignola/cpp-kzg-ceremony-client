#ifndef BLS_SIGNATURE_HPP
#define BLS_SIGNATURE_HPP

#include <blst.hpp>
#include <string>

#ifdef _DLL
#undef _DLL
#include <uint256_t.h>
#define _DLL
#else
#include <uint256_t.h>
#endif

class BlsSignature {
public:
  BlsSignature(uint256_t secret, const std::string& identity);
  std::string encode() const;

private:
  blst::P1_Affine bls_signature_;
};

#endif // BLS_SIGNATURE_HPP