// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/crc32.h"

#include "sm/base/arraysize.h"

namespace sm {

// This implementation is based on the sample implementation in RFC 1952.

// CRC32 polynomial, in reversed form.
// See RFC 1952, or http://en.wikipedia.org/wiki/Cyclic_redundancy_check
static const uint32_t kCrc32Polynomial = 0xEDB88320;
static uint32_t kCrc32Table[256] = {0};

static void EnsureCrc32TableInited() {
  if (kCrc32Table[arraysize(kCrc32Table) - 1])
    return;  // already inited
  for (uint32_t i = 0; i < arraysize(kCrc32Table); ++i) {
    uint32_t c = i;
    for (size_t j = 0; j < 8; ++j) {
      if (c & 1) {
        c = kCrc32Polynomial ^ (c >> 1);
      } else {
        c >>= 1;
      }
    }
    kCrc32Table[i] = c;
  }
}

uint32_t UpdateCrc32(uint32_t start, const void* buf, size_t len) {
  EnsureCrc32TableInited();

  uint32_t c = start ^ 0xFFFFFFFF;
  const uint8_t* u = static_cast<const uint8_t*>(buf);
  for (size_t i = 0; i < len; ++i) {
    c = kCrc32Table[(c ^ u[i]) & 0xFF] ^ (c >> 8);
  }
  return c ^ 0xFFFFFFFF;
}

}  // namespace sm

