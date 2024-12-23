// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_CPU_INFO_H_
#define SM_BASE_CPU_INFO_H_

#include "sm/typedefs.h"

namespace sm {

class CpuInfo {
 public:
  static uint32_t DetectNumberOfCores();

 private:
  CpuInfo() {}
};

}  // namespace sm

#endif // SM_BASE_CPU_INFO_H_
