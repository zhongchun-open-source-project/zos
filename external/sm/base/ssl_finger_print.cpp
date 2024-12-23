// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/ssl_finger_print.h"

#include <ctype.h>
#include <string>

#include "sm/base/helpers.h"
#include "sm/base/message_digest.h"
#include "sm/base/string_encode.h"

namespace sm {

SSLFingerprint* SSLFingerprint::Create(
    const std::string& algorithm, const SSLIdentity* identity) {
  if (!identity) {
    return NULL;
  }

  return Create(algorithm, &(identity->certificate()));
}

SSLFingerprint* SSLFingerprint::Create(
    const std::string& algorithm, const SSLCertificate* cert) {
  uint8_t digest_val[64];
  size_t digest_len;
  bool ret = cert->ComputeDigest(
      algorithm, digest_val, sizeof(digest_val), &digest_len);
  if (!ret) {
    return NULL;
  }

  return new SSLFingerprint(algorithm, digest_val, digest_len);
}

SSLFingerprint* SSLFingerprint::CreateFromRfc4572(
    const std::string& algorithm, const std::string& fingerprint) {
  if (algorithm.empty() || !IsFips180DigestAlgorithm(algorithm))
    return NULL;

  if (fingerprint.empty())
    return NULL;

  size_t value_len;
  char value[MessageDigest::kMaxSize];
  value_len = hex_decode_with_delimiter(value, sizeof(value),
                                                   fingerprint.c_str(),
                                                   fingerprint.length(),
                                                   ':');
  if (!value_len)
    return NULL;

  return new SSLFingerprint(algorithm, reinterpret_cast<uint8_t*>(value),
                            value_len);
}

SSLFingerprint::SSLFingerprint(const std::string& algorithm,
                               const uint8_t* digest_in,
                               size_t digest_len)
    : algorithm(algorithm) {
  digest.SetData(digest_in, digest_len);
}

SSLFingerprint::SSLFingerprint(const SSLFingerprint& from)
    : algorithm(from.algorithm), digest(from.digest) {}

bool SSLFingerprint::operator==(const SSLFingerprint& other) const {
  return algorithm == other.algorithm &&
         digest == other.digest;
}

std::string SSLFingerprint::GetRfc4572Fingerprint() const {
  std::string fingerprint =
      hex_encode_with_delimiter(digest.data<char>(), digest.size(), ':');
  std::transform(fingerprint.begin(), fingerprint.end(),
                 fingerprint.begin(), ::toupper);
  return fingerprint;
}

std::string SSLFingerprint::ToString() {
  std::string fp_str = algorithm;
  fp_str.append(" ");
  fp_str.append(GetRfc4572Fingerprint());
  return fp_str;
}

}  // namespace sm
