// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include <cpu-features.h>

uint64_t Sm_GetCPUFeaturesARM(void) {
  return android_getCpuFeatures();
}
