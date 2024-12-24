// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_MD5_DIGEST_H_
#define SM_BASE_MD5_DIGEST_H_

#include "sm/base/md5.h"
#include "sm/base/message_digest.h"

namespace sm {

// A simple wrapper for our MD5 implementation.
class Md5Digest : public MessageDigest {
 public:
  enum { kSize = 16 };
  Md5Digest() {
    MD5Init(&ctx_);
  }
  size_t Size() const override;
  void Update(const void* buf, size_t len) override;
  size_t Finish(void* buf, size_t len) override;

 private:
  MD5Context ctx_;
};

}  // namespace sm

#endif  // SM_BASE_MD5_DIGEST_H_
