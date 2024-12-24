// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_WIN32_WINDOW_PICKER_H_
#define SM_BASE_WIN32_WINDOW_PICKER_H_

#include "sm/base/win32.h"
#include "sm/base/window_picker.h"

namespace sm {

class Win32WindowPicker : public WindowPicker {
 public:
  Win32WindowPicker();
  virtual bool Init();
  virtual bool IsVisible(const WindowId& id);
  virtual bool MoveToFront(const WindowId& id);
  virtual bool GetWindowList(WindowDescriptionList* descriptions);
  virtual bool GetDesktopList(DesktopDescriptionList* descriptions);
  virtual bool GetDesktopDimensions(const DesktopId& id, int* width,
                                    int* height);

 protected:
  static BOOL CALLBACK EnumProc(HWND hwnd, LPARAM l_param);
  static BOOL CALLBACK MonitorEnumProc(HMONITOR h_monitor,
                                       HDC hdc_monitor,
                                       LPRECT lprc_monitor,
                                       LPARAM l_param);
};

}  // namespace sm

#endif  // SM_BASE_WIN32_WINDOW_PICKER_H_
