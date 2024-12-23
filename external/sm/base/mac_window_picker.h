// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_MAC_WINDOW_PICKER_H_
#define SM_BASE_MAC_WINDOW_PICKER_H_

#include "sm/base/window_picker.h"

namespace sm {

class MacWindowPicker : public WindowPicker {
 public:
  MacWindowPicker();
  ~MacWindowPicker() override;
  bool Init() override;
  bool IsVisible(const WindowId& id) override;
  bool MoveToFront(const WindowId& id) override;
  bool GetWindowList(WindowDescriptionList* descriptions) override;
  bool GetDesktopList(DesktopDescriptionList* descriptions) override;
  bool GetDesktopDimensions(const DesktopId& id,
                            int* width,
                            int* height) override;

 private:
  void* lib_handle_;
  void* get_window_list_;
  void* get_window_list_desc_;
};

}  // namespace sm

#endif  // SM_BASE_MAC_WINDOW_PICKER_H_
