// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_SSL_FINGER_PRINT_H_
#define SM_BASE_SSL_FINGER_PRINT_H_

#include <string>

#include "sm/base/basic_types.h"
#include "sm/base/buffer.h"
#include "sm/base/ssl_identity.h"

namespace sm {

class SSLCertificate;

struct SSLFingerprint {
  static SSLFingerprint* Create(const std::string& algorithm,
                                const SSLIdentity* identity);

  static SSLFingerprint* Create(const std::string& algorithm,
                                const SSLCertificate* cert);

  static SSLFingerprint* CreateFromRfc4572(const std::string& algorithm,
                                           const std::string& fingerprint);

  SSLFingerprint(const std::string& algorithm,
                 const uint8_t* digest_in,
                 size_t digest_len);

  SSLFingerprint(const SSLFingerprint& from);

  bool operator==(const SSLFingerprint& other) const;

  std::string GetRfc4572Fingerprint() const;

  std::string ToString();

  std::string algorithm;
  Buffer digest;
};

}  // namespace sm

#endif  // SM_BASE_SSL_FINGER_PRINT_H_
