// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/platform_thread.h"

#include "testing/gtest/include/gtest/gtest.h"
#include "sm/base/scoped_ptr.h"
#include "sm/base/sleep.h"

namespace sm {

// Function that does nothing, and reports success.
bool NullRunFunction(void* obj) {
  SleepMs(0);  // Hand over timeslice, prevents busy looping.
  return true;
}

TEST(PlatformThreadTest, StartStop) {
  PlatformThread thread(&NullRunFunction, nullptr, "PlatformThreadTest");
  thread.Start();
  thread.Stop();
}

// Function that sets a boolean.
bool SetFlagRunFunction(void* obj) {
  bool* obj_as_bool = static_cast<bool*>(obj);
  *obj_as_bool = true;
  SleepMs(0);  // Hand over timeslice, prevents busy looping.
  return true;
}

TEST(PlatformThreadTest, RunFunctionIsCalled) {
  bool flag = false;
  PlatformThread thread(&SetFlagRunFunction, &flag, "RunFunctionIsCalled");
  thread.Start();

  // At this point, the flag may be either true or false.
  thread.Stop();

  // We expect the thread to have run at least once.
  EXPECT_TRUE(flag);
}

}  // namespace sm
