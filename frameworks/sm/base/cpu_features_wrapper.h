// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_CPU_FEATURES_WRAPPER_H_
#define SM_BASE_CPU_FEATURES_WRAPPER_H_

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include "sm/typedefs.h"

// List of features in x86.
typedef enum {
  kSSE2,
  kSSE3
} CPUFeature;

// List of features in ARM.
enum {
  kCPUFeatureARMv7       = (1 << 0),
  kCPUFeatureVFPv3       = (1 << 1),
  kCPUFeatureNEON        = (1 << 2),
  kCPUFeatureLDREXSTREX  = (1 << 3)
};

typedef int (*Sm_CPUInfo)(CPUFeature feature);

// Returns true if the CPU supports the feature.
extern Sm_CPUInfo Sm_GetCPUInfo;

// No CPU feature is available => straight C path.
extern Sm_CPUInfo Sm_GetCPUInfoNoASM;

// Return the features in an ARM device.
// It detects the features in the hardware platform, and returns supported
// values in the above enum definition as a bitmask.
extern uint64_t Sm_GetCPUFeaturesARM(void);

#if defined(__cplusplus) || defined(c_plusplus)
}  // extern "C"
#endif

#endif // SM_BASE_CPU_FEATURES_WRAPPER_H_
