// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_OPENSSL_DIGEST_H_
#define SM_BASE_OPENSSL_DIGEST_H_

#include <openssl/evp.h>

#include "sm/base/message_digest.h"

namespace sm {

// An implementation of the digest class that uses OpenSSL.
class OpenSSLDigest : public MessageDigest {
 public:
  // Creates an OpenSSLDigest with |algorithm| as the hash algorithm.
  explicit OpenSSLDigest(const std::string& algorithm);
  ~OpenSSLDigest() override;
  // Returns the digest output size (e.g. 16 bytes for MD5).
  size_t Size() const override;
  // Updates the digest with |len| bytes from |buf|.
  void Update(const void* buf, size_t len) override;
  // Outputs the digest value to |buf| with length |len|.
  size_t Finish(void* buf, size_t len) override;

  // Helper function to look up a digest's EVP by name.
  static bool GetDigestEVP(const std::string &algorithm,
                           const EVP_MD** md);
  // Helper function to look up a digest's name by EVP.
  static bool GetDigestName(const EVP_MD* md,
                            std::string* algorithm);
  // Helper function to get the length of a digest.
  static bool GetDigestSize(const std::string &algorithm,
                            size_t* len);

 private:
  EVP_MD_CTX ctx_;
  const EVP_MD* md_;
};

}  // namespace sm

#endif  // SM_BASE_OPENSSL_DIGEST_H_
