// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/gunit.h"
#include "sm/base/test_utils.h"
#include "sm/base/window.h"
#include "sm/base/window_picker.h"
#include "sm/base/window_picker_factory.h"

#if defined(SM_MAC) && !defined(SM_IOS)
#  define DISABLE_ON_MAC(name) DISABLED_ ## name
#else
#  define DISABLE_ON_MAC(name) name
#endif

TEST(WindowPickerTest, GetWindowList) {
  MAYBE_SKIP_SCREENCAST_TEST();
  if (!WindowPickerFactory::IsSupported()) {
    LOG(LS_INFO) << "skipping test: window capturing is not supported with "
                 << "current configuration.";
  }
  scoped_ptr<WindowPicker> picker(
      WindowPickerFactory::CreateWindowPicker());
  EXPECT_TRUE(picker->Init());
  WindowDescriptionList descriptions;
  EXPECT_TRUE(picker->GetWindowList(&descriptions));
}

// TODO(hughv) Investigate why this fails on pulse but not locally after
// upgrading to XCode 4.5.  The failure is GetDesktopList returning FALSE.
TEST(WindowPickerTest, DISABLE_ON_MAC(GetDesktopList)) {
  MAYBE_SKIP_SCREENCAST_TEST();
  if (!WindowPickerFactory::IsSupported()) {
    LOG(LS_INFO) << "skipping test: window capturing is not supported with "
                 << "current configuration.";
  }
  scoped_ptr<WindowPicker> picker(
      WindowPickerFactory::CreateWindowPicker());
  EXPECT_TRUE(picker->Init());
  DesktopDescriptionList descriptions;
  EXPECT_TRUE(picker->GetDesktopList(&descriptions));
  if (descriptions.size() > 0) {
    int width = 0;
    int height = 0;
    EXPECT_TRUE(picker->GetDesktopDimensions(descriptions[0].id(), &width,
                                             &height));
    EXPECT_GT(width, 0);
    EXPECT_GT(height, 0);

    // Test |IsPrimaryDesktop|. Only one desktop should be a primary.
    bool found_primary = false;
    for (DesktopDescriptionList::iterator it = descriptions.begin();
         it != descriptions.end(); ++it) {
      if (it->primary()) {
        EXPECT_FALSE(found_primary);
        found_primary = true;
      }
    }
    EXPECT_TRUE(found_primary);
  }
}
