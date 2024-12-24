// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/gunit.h"
#include "sm/base/logging.h"
#include "sm/base/mac_utils.h"
#include "sm/base/mac_window_picker.h"
#include "sm/base/window_picker.h"

#if !defined(SM_MAC) || defined(SM_IOS)
#error Only for SM_MAC && !SM_IOS
#endif

namespace sm {

bool IsLeopardOrLater() {
  return GetOSVersionName() >= kMacOSLeopard;
}

// Test that this works on new versions and fails acceptably on old versions.
TEST(MacWindowPickerTest, TestGetWindowList) {
  MacWindowPicker picker, picker2;
  WindowDescriptionList descriptions;
  if (IsLeopardOrLater()) {
    EXPECT_TRUE(picker.Init());
    EXPECT_TRUE(picker.GetWindowList(&descriptions));
    EXPECT_TRUE(picker2.GetWindowList(&descriptions));  // Init is optional
  } else {
    EXPECT_FALSE(picker.Init());
    EXPECT_FALSE(picker.GetWindowList(&descriptions));
    EXPECT_FALSE(picker2.GetWindowList(&descriptions));
  }
}

// TODO: Add verification of the actual parsing, ie, add
// functionality to inject a fake get_window_array function which
// provide a pre-constructed list of windows.

}  // namespace sm
