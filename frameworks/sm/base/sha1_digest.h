// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_SHA1_DIGEST_H_
#define SM_BASE_SHA1_DIGEST_H_

#include "sm/base/message_digest.h"
#include "sm/base/sha1.h"

namespace sm {

// A simple wrapper for our SHA-1 implementation.
class Sha1Digest : public MessageDigest {
 public:
  enum { kSize = SHA1_DIGEST_SIZE };
  Sha1Digest() {
    SHA1Init(&ctx_);
  }
  size_t Size() const override;
  void Update(const void* buf, size_t len) override;
  size_t Finish(void* buf, size_t len) override;

 private:
  SHA1_CTX ctx_;
};

}  // namespace sm

#endif  // SM_BASE_SHA1_DIGEST_H_
