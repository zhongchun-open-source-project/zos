// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_WINDOW_H_
#define SM_BASE_WINDOW_H_

#include "sm/base/basic_types.h"
#include "sm/base/string_encode.h"

// Define platform specific window types.
#if defined(SM_LINUX) && !defined(SM_ANDROID)
typedef unsigned long Window;  // Avoid include <X11/Xlib.h>.
#elif defined(SM_WIN)
// We commonly include win32.h in sm/base so just include it here.
#include "sm/base/win32.h"  // Include HWND, HMONITOR.
#elif defined(SM_MAC) && !defined(SM_IOS)
typedef unsigned int CGWindowID;
typedef unsigned int CGDirectDisplayID;
#endif

namespace sm {

class WindowId {
 public:
  // Define WindowT for each platform.
#if defined(SM_LINUX) && !defined(SM_ANDROID)
  typedef Window WindowT;
#elif defined(SM_WIN)
  typedef HWND WindowT;
#elif defined(SM_MAC) && !defined(SM_IOS)
  typedef CGWindowID WindowT;
#else
  typedef unsigned int WindowT;
#endif

  static WindowId Cast(uint64_t id) {
#if defined(SM_WIN)
    return WindowId(reinterpret_cast<WindowId::WindowT>(id));
#else
    return WindowId(static_cast<WindowId::WindowT>(id));
#endif
  }

  static uint64_t Format(const WindowT& id) {
#if defined(SM_WIN)
    return static_cast<uint64_t>(reinterpret_cast<uintptr_t>(id));
#else
    return static_cast<uint64_t>(id);
#endif
  }

  WindowId() : id_(0) {}
  WindowId(const WindowT& id) : id_(id) {}  // NOLINT
  const WindowT& id() const { return id_; }
  bool IsValid() const { return id_ != 0; }
  bool Equals(const WindowId& other) const {
    return id_ == other.id();
  }

 private:
  WindowT id_;
};

class DesktopId {
 public:
  // Define DesktopT for each platform.
#if defined(SM_LINUX) && !defined(SM_ANDROID)
  typedef Window DesktopT;
#elif defined(SM_WIN)
  typedef HMONITOR DesktopT;
#elif defined(SM_MAC) && !defined(SM_IOS)
  typedef CGDirectDisplayID DesktopT;
#else
  typedef unsigned int DesktopT;
#endif

  static DesktopId Cast(int id, int index) {
#if defined(SM_WIN)
    return DesktopId(reinterpret_cast<DesktopId::DesktopT>(id), index);
#else
    return DesktopId(static_cast<DesktopId::DesktopT>(id), index);
#endif
  }

  DesktopId() : id_(0), index_(-1) {}
  DesktopId(const DesktopT& id, int index)  // NOLINT
      : id_(id), index_(index) {
  }
  const DesktopT& id() const { return id_; }
  int index() const { return index_; }
  bool IsValid() const { return index_ != -1; }
  bool Equals(const DesktopId& other) const {
    return id_ == other.id() && index_ == other.index();
  }

 private:
  // Id is the platform specific desktop identifier.
  DesktopT id_;
  // Index is the desktop index as enumerated by each platform.
  // Desktop capturer typically takes the index instead of id.
  int index_;
};

// Window event types.
enum WindowEvent {
  WE_RESIZE = 0,
  WE_CLOSE = 1,
  WE_MINIMIZE = 2,
  WE_RESTORE = 3,
};

inline std::string ToString(const WindowId& window) {
  return ToString(window.id());
}

}  // namespace sm

#endif  // SM_BASE_WINDOW_H_
