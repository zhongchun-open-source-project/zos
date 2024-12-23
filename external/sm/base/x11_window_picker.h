// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_LINUX_WINDOW_PICKER_H_
#define SM_BASE_LINUX_WINDOW_PICKER_H_

#include "sm/base/basic_types.h"
#include "sm/base/scoped_ptr.h"
#include "sm/base/window_picker.h"

// Avoid include <X11/Xlib.h>.
struct _XDisplay;
typedef unsigned long Window;

namespace sm {

class XWindowEnumerator;

class X11WindowPicker : public WindowPicker {
 public:
  X11WindowPicker();
  ~X11WindowPicker() override;

  static bool IsDesktopElement(_XDisplay* display, Window window);

  bool Init() override;
  bool IsVisible(const WindowId& id) override;
  bool MoveToFront(const WindowId& id) override;
  bool GetWindowList(WindowDescriptionList* descriptions) override;
  bool GetDesktopList(DesktopDescriptionList* descriptions) override;
  bool GetDesktopDimensions(const DesktopId& id,
                            int* width,
                            int* height) override;
  uint8_t* GetWindowIcon(const WindowId& id, int* width, int* height);
  uint8_t* GetWindowThumbnail(const WindowId& id, int width, int height);
  int GetNumDesktops();
  uint8_t* GetDesktopThumbnail(const DesktopId& id, int width, int height);

 private:
  scoped_ptr<XWindowEnumerator> enumerator_;
};

}  // namespace sm

#endif  // SM_BASE_LINUX_WINDOW_PICKER_H_
