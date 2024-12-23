// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/sha1_digest.h"

namespace sm {

size_t Sha1Digest::Size() const {
  return kSize;
}

void Sha1Digest::Update(const void* buf, size_t len) {
  SHA1Update(&ctx_, static_cast<const uint8_t*>(buf), len);
}

size_t Sha1Digest::Finish(void* buf, size_t len) {
  if (len < kSize) {
    return 0;
  }
  SHA1Final(&ctx_, static_cast<uint8_t*>(buf));
  SHA1Init(&ctx_);  // Reset for next use.
  return kSize;
}

}  // namespace sm
