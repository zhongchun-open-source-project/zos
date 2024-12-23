// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/cpu_info.h"

#include "sm/base/system_info.h"

namespace sm {

uint32_t CpuInfo::DetectNumberOfCores() {
  return static_cast<uint32_t>(SystemInfo::GetMaxCpus());
}

}  // namespace sm
