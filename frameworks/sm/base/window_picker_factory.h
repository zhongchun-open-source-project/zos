// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_WINDOW_PICKER_FACTORY_H_
#define SM_BASE_WINDOW_PICKER_FACTORY_H_

#if defined(SM_WIN)
#include "sm/base/win32_window_picker.h"
#elif defined(SM_MAC) && !defined(SM_IOS)
#include "sm/base/mac_utils.h"
#include "sm/base/mac_window_picker.h"
#elif defined(SM_LINUX) && !defined(SM_ANDROID) && defined(HAVE_X11)
#include "sm/base/x11_window_picker.h"
#endif

#include "sm/base/window_picker.h"

namespace sm {

class WindowPickerFactory {
 public:
  virtual ~WindowPickerFactory() {}

  // Instance method for dependency injection.
  virtual WindowPicker* Create() {
    return CreateWindowPicker();
  }

  static WindowPicker* CreateWindowPicker() {
#if defined(SM_WIN)
    return new Win32WindowPicker();
#elif defined(SM_MAC) && !defined(SM_IOS)
    return new MacWindowPicker();
#elif defined(SM_LINUX) && !defined(SM_ANDROID) && defined(HAVE_X11)
    return new X11WindowPicker();
#else
    return NULL;
#endif
  }

  static bool IsSupported() {
#if defined(SM_MAC) && !defined(SM_IOS)
    return GetOSVersionName() >= kMacOSLeopard;
#else
    return true;
#endif
  }
};

}  // namespace sm

#endif  // SM_BASE_WINDOW_PICKER_FACTORY_H_
