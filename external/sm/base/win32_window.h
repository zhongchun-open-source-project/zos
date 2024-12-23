// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_WIN32_WINDOW_H_
#define SM_BASE_WIN32_WINDOW_H_

#if defined(SM_WIN)

#include "sm/base/win32.h"

namespace sm {

///////////////////////////////////////////////////////////////////////////////
// Win32Window
///////////////////////////////////////////////////////////////////////////////

class Win32Window {
 public:
  Win32Window();
  virtual ~Win32Window();

  HWND handle() const { return wnd_; }

  bool Create(HWND parent, const wchar_t* title, DWORD style, DWORD exstyle,
              int x, int y, int cx, int cy);
  void Destroy();

  // Call this when your DLL unloads.
  static void Shutdown();

 protected:
  virtual bool OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
                         LRESULT& result);

  virtual bool OnClose() { return true; }
  virtual void OnNcDestroy() { }

 private:
  static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                                  LPARAM lParam);

  HWND wnd_;
  static HINSTANCE instance_;
  static ATOM window_class_;
};

///////////////////////////////////////////////////////////////////////////////

}  // namespace sm

#endif  // SM_WIN 

#endif  // SM_BASE_WIN32_WINDOW_H_
