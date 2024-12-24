// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/md5_digest.h"

namespace sm {

size_t Md5Digest::Size() const {
  return kSize;
}

void Md5Digest::Update(const void* buf, size_t len) {
  MD5Update(&ctx_, static_cast<const uint8_t*>(buf), len);
}

size_t Md5Digest::Finish(void* buf, size_t len) {
  if (len < kSize) {
    return 0;
  }
  MD5Final(&ctx_, static_cast<uint8_t*>(buf));
  MD5Init(&ctx_);  // Reset for next use.
  return kSize;
}

};  // namespace sm
