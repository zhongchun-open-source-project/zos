// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/gunit.h"
#include "sm/base/string_utils.h"
#include "sm/base/system_info.h"

#if defined(CPU_X86) || defined(CPU_ARM)
TEST(SystemInfoTest, CpuVendorNonEmpty) {
  SystemInfo info;
  LOG(LS_INFO) << "CpuVendor: " << info.GetCpuVendor();
  EXPECT_FALSE(info.GetCpuVendor().empty());
}

// Tests Vendor identification is Intel or AMD.
// See Also http://en.wikipedia.org/wiki/CPUID
TEST(SystemInfoTest, CpuVendorIntelAMDARM) {
  SystemInfo info;
#if defined(CPU_X86)
  EXPECT_TRUE(string_match(info.GetCpuVendor().c_str(),
                                      "GenuineIntel") ||
              string_match(info.GetCpuVendor().c_str(),
                                      "AuthenticAMD"));
#elif defined(CPU_ARM)
  EXPECT_TRUE(string_match(info.GetCpuVendor().c_str(), "ARM"));
#endif
}
#endif  // defined(CPU_X86) || defined(CPU_ARM)

// Tests CpuArchitecture matches expectations.
TEST(SystemInfoTest, GetCpuArchitecture) {
  SystemInfo info;
  LOG(LS_INFO) << "CpuArchitecture: " << info.GetCpuArchitecture();
  SystemInfo::Architecture architecture = info.GetCpuArchitecture();
#if defined(CPU_X86) || defined(CPU_ARM)
  if (sizeof(intptr_t) == 8) {
    EXPECT_EQ(SystemInfo::SI_ARCH_X64, architecture);
  } else if (sizeof(intptr_t) == 4) {
#if defined(CPU_ARM)
    EXPECT_EQ(SystemInfo::SI_ARCH_ARM, architecture);
#else
    EXPECT_EQ(SystemInfo::SI_ARCH_X86, architecture);
#endif
  }
#endif
}

// Tests MachineModel is set.  On Mac test machine model is known.
TEST(SystemInfoTest, MachineModelKnown) {
  SystemInfo info;
  EXPECT_FALSE(info.GetMachineModel().empty());
  const char *machine_model = info.GetMachineModel().c_str();
  LOG(LS_INFO) << "MachineModel: " << machine_model;
  bool known = true;
#if defined(SM_MAC) && !defined(SM_IOS)
  // Full list as of May 2012.  Update when new OSX based models are added.
  known = string_match(machine_model, "MacBookPro*") ||
          string_match(machine_model, "MacBookAir*") ||
          string_match(machine_model, "MacBook*") ||
          string_match(machine_model, "MacPro*") ||
          string_match(machine_model, "Macmini*") ||
          string_match(machine_model, "iMac*") ||
          string_match(machine_model, "Xserve*");
#elif !defined(SM_IOS)
  // All other machines return Not available.
  known = string_match(info.GetMachineModel().c_str(),
                                  "Not available");
#endif
  if (!known) {
    LOG(LS_WARNING) << "Machine Model Unknown: " << machine_model;
  }
}

// Tests physical memory size.
TEST(SystemInfoTest, MemorySize) {
  SystemInfo info;
  LOG(LS_INFO) << "MemorySize: " << info.GetMemorySize();
  EXPECT_GT(info.GetMemorySize(), -1);
}

// Tests number of logical cpus available to the system.
TEST(SystemInfoTest, MaxCpus) {
  SystemInfo info;
  LOG(LS_INFO) << "MaxCpus: " << info.GetMaxCpus();
  EXPECT_GT(info.GetMaxCpus(), 0);
}

// Tests number of logical cpus available to the process.
TEST(SystemInfoTest, CurCpus) {
  SystemInfo info;
  LOG(LS_INFO) << "CurCpus: " << info.GetCurCpus();
  EXPECT_GT(info.GetCurCpus(), 0);
  EXPECT_LE(info.GetCurCpus(), info.GetMaxCpus());
}

#ifdef CPU_X86
// CPU family/model/stepping is only available on X86. The following tests
// that they are set when running on x86 CPUs. Valid Family/Model/Stepping
// values are non-zero on known CPUs.

// Tests Intel CPU Family identification.
TEST(SystemInfoTest, CpuFamily) {
  SystemInfo info;
  LOG(LS_INFO) << "CpuFamily: " << info.GetCpuFamily();
  EXPECT_GT(info.GetCpuFamily(), 0);
}

// Tests Intel CPU Model identification.
TEST(SystemInfoTest, CpuModel) {
  SystemInfo info;
  LOG(LS_INFO) << "CpuModel: " << info.GetCpuModel();
  EXPECT_GT(info.GetCpuModel(), 0);
}

// Tests Intel CPU Stepping identification.
TEST(SystemInfoTest, CpuStepping) {
  SystemInfo info;
  LOG(LS_INFO) << "CpuStepping: " << info.GetCpuStepping();
  EXPECT_GT(info.GetCpuStepping(), 0);
}
#else  // CPU_X86
// If not running on x86 CPU the following tests expect the functions to
// return 0.
TEST(SystemInfoTest, CpuFamily) {
  SystemInfo info;
  LOG(LS_INFO) << "CpuFamily: " << info.GetCpuFamily();
  EXPECT_EQ(0, info.GetCpuFamily());
}

// Tests Intel CPU Model identification.
TEST(SystemInfoTest, CpuModel) {
  SystemInfo info;
  LOG(LS_INFO) << "CpuModel: " << info.GetCpuModel();
  EXPECT_EQ(0, info.GetCpuModel());
}

// Tests Intel CPU Stepping identification.
TEST(SystemInfoTest, CpuStepping) {
  SystemInfo info;
  LOG(LS_INFO) << "CpuStepping: " << info.GetCpuStepping();
  EXPECT_EQ(0, info.GetCpuStepping());
}
#endif  // CPU_X86
