// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/gunit.h"
#include "sm/base/mac_utils.h"

TEST(MacUtilsTest, GetOsVersionName) {
  MacOSVersionName ver = GetOSVersionName();
  LOG(LS_INFO) << "GetOsVersionName " << ver;
  EXPECT_NE(kMacOSUnknown, ver);
}

TEST(MacUtilsTest, GetQuickTimeVersion) {
  std::string version;
  EXPECT_TRUE(GetQuickTimeVersion(&version));
  LOG(LS_INFO) << "GetQuickTimeVersion " << version;
}

TEST(MacUtilsTest, RunAppleScriptCompileError) {
  std::string script("set value to to 5");
  EXPECT_FALSE(RunAppleScript(script));
}

TEST(MacUtilsTest, RunAppleScriptRuntimeError) {
  std::string script("set value to 5 / 0");
  EXPECT_FALSE(RunAppleScript(script));
}

#ifdef CARBON_DEPRECATED
TEST(MacUtilsTest, DISABLED_RunAppleScriptSuccess) {
#else
TEST(MacUtilsTest, RunAppleScriptSuccess) {
#endif
  std::string script("set value to 5");
  EXPECT_TRUE(RunAppleScript(script));
}
