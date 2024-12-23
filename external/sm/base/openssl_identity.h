// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_OPENSSL_IDENTITY_H_
#define SM_BASE_OPENSSL_IDENTITY_H_

#include <openssl/evp.h>
#include <openssl/x509.h>

#include <string>

#include "sm/base/common.h"
#include "sm/base/scoped_ptr.h"
#include "sm/base/ssl_identity.h"

typedef struct ssl_ctx_st SSL_CTX;

namespace sm {

// OpenSSLKeyPair encapsulates an OpenSSL EVP_PKEY* keypair object,
// which is reference counted inside the OpenSSL library.
class OpenSSLKeyPair {
 public:
  explicit OpenSSLKeyPair(EVP_PKEY* pkey) : pkey_(pkey) {
    ASSERT(pkey_ != NULL);
  }

  static OpenSSLKeyPair* Generate(const KeyParams& key_params);

  virtual ~OpenSSLKeyPair();

  virtual OpenSSLKeyPair* GetReference();

  EVP_PKEY* pkey() const { return pkey_; }

 private:
  void AddReference();

  EVP_PKEY* pkey_;

  SM_DISALLOW_COPY_AND_ASSIGN(OpenSSLKeyPair);
};

// OpenSSLCertificate encapsulates an OpenSSL X509* certificate object,
// which is also reference counted inside the OpenSSL library.
class OpenSSLCertificate : public SSLCertificate {
 public:
  // Caller retains ownership of the X509 object.
  explicit OpenSSLCertificate(X509* x509) : x509_(x509) {
    AddReference();
  }

  static OpenSSLCertificate* Generate(OpenSSLKeyPair* key_pair,
                                      const SSLIdentityParams& params);
  static OpenSSLCertificate* FromPEMString(const std::string& pem_string);

  ~OpenSSLCertificate() override;

  OpenSSLCertificate* GetReference() const override;

  X509* x509() const { return x509_; }

  std::string ToPEMString() const override;

  void ToDER(Buffer* der_buffer) const override;

  // Compute the digest of the certificate given algorithm
  bool ComputeDigest(const std::string& algorithm,
                     unsigned char* digest,
                     size_t size,
                     size_t* length) const override;

  // Compute the digest of a certificate as an X509 *
  static bool ComputeDigest(const X509* x509,
                            const std::string& algorithm,
                            unsigned char* digest,
                            size_t size,
                            size_t* length);

  bool GetSignatureDigestAlgorithm(std::string* algorithm) const override;
  bool GetChain(SSLCertChain** chain) const override;

  int64_t CertificateExpirationTime() const override;

 private:
  void AddReference() const;

  X509* x509_;

  SM_DISALLOW_COPY_AND_ASSIGN(OpenSSLCertificate);
};

// Holds a keypair and certificate together, and a method to generate
// them consistently.
class OpenSSLIdentity : public SSLIdentity {
 public:
  static OpenSSLIdentity* Generate(const std::string& common_name,
                                   const KeyParams& key_params);
  static OpenSSLIdentity* GenerateForTest(const SSLIdentityParams& params);
  static SSLIdentity* FromPEMStrings(const std::string& private_key,
                                     const std::string& certificate);
  ~OpenSSLIdentity() override;

  const OpenSSLCertificate& certificate() const override;
  OpenSSLIdentity* GetReference() const override;

  // Configure an SSL context object to use our key and certificate.
  bool ConfigureIdentity(SSL_CTX* ctx);

 private:
  OpenSSLIdentity(OpenSSLKeyPair* key_pair, OpenSSLCertificate* certificate);

  static OpenSSLIdentity* GenerateInternal(const SSLIdentityParams& params);

  scoped_ptr<OpenSSLKeyPair> key_pair_;
  scoped_ptr<OpenSSLCertificate> certificate_;

  SM_DISALLOW_COPY_AND_ASSIGN(OpenSSLIdentity);
};


}  // namespace sm

#endif  // SM_BASE_OPENSSL_IDENTITY_H_
