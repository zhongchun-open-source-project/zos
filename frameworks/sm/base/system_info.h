// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_SYSTEM_INFO_H_
#define SM_BASE_SYSTEM_INFO_H_

#include <string>

#include "sm/base/basictypes.h"

namespace sm {

class SystemInfo {
 public:
  enum Architecture {
    SI_ARCH_UNKNOWN = -1,
    SI_ARCH_X86 = 0,
    SI_ARCH_X64 = 1,
    SI_ARCH_ARM = 2
  };

  SystemInfo();

  // The number of CPU Threads in the system.
  static int GetMaxCpus();
  // The number of CPU Threads currently available to this process.
  static int GetCurCpus();
  // Identity of the CPUs.
  Architecture GetCpuArchitecture();
  std::string GetCpuVendor();
  // Total amount of physical memory, in bytes.
  int64_t GetMemorySize();
  // The model name of the machine, e.g. "MacBookAir1,1"
  std::string GetMachineModel();

 private:
  static int logical_cpus_;
};

}  // namespace sm

#endif  // SM_BASE_SYSTEM_INFO_H_
