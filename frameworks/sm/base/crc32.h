// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_CRC32_H_
#define SM_BASE_CRC32_H_

#include <string>

#include "sm/base/basic_types.h"

namespace sm {

// Updates a CRC32 checksum with |len| bytes from |buf|. |initial| holds the
// checksum result from the previous update; for the first call, it should be 0.
uint32_t UpdateCrc32(uint32_t initial, const void* buf, size_t len);

// Computes a CRC32 checksum using |len| bytes from |buf|.
inline uint32_t ComputeCrc32(const void* buf, size_t len) {
  return UpdateCrc32(0, buf, len);
}
inline uint32_t ComputeCrc32(const std::string& str) {
  return ComputeCrc32(str.c_str(), str.size());
}

}  // namespace sm

#endif  // SM_BASE_CRC32_H_
