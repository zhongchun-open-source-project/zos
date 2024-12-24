// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/gunit.h"
#include "sm/base/x11_window_picker.h"
#include "sm/base/logging.h"
#include "sm/base/test_utils.h"
#include "sm/base/window_picker.h"

#if !defined(SM_LINUX) || defined(SM_ANDROID)
#error Only for Linux
#endif

namespace sm {

TEST(X11WindowPickerTest, TestGetWindowList) {
  MAYBE_SKIP_SCREENCAST_TEST();
  X11WindowPicker window_picker;
  WindowDescriptionList descriptions;
  window_picker.Init();
  window_picker.GetWindowList(&descriptions);
}

TEST(X11WindowPickerTest, TestGetDesktopList) {
  MAYBE_SKIP_SCREENCAST_TEST();
  X11WindowPicker window_picker;
  DesktopDescriptionList descriptions;
  EXPECT_TRUE(window_picker.Init());
  EXPECT_TRUE(window_picker.GetDesktopList(&descriptions));
  EXPECT_TRUE(descriptions.size() > 0);
}

}  // namespace sm
