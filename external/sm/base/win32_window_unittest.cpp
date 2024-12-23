// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/gunit.h"
#include "sm/base/common.h"
#include "sm/base/win32_window.h"
#include "sm/base/logging.h"

static LRESULT kDummyResult = 0x1234ABCD;

class TestWindow : public Win32Window {
 public:
  TestWindow() : destroyed_(false) { memset(&msg_, 0, sizeof(msg_)); }
  const MSG& msg() const { return msg_; }
  bool destroyed() const { return destroyed_; }

  virtual bool OnMessage(UINT uMsg, WPARAM wParam,
                         LPARAM lParam, LRESULT& result) {
    msg_.message = uMsg;
    msg_.wParam = wParam;
    msg_.lParam = lParam;
    result = kDummyResult;
    return true;
  }
  virtual void OnNcDestroy() {
    destroyed_ = true;
  }

 private:
  MSG msg_;
  bool destroyed_;
};

TEST(Win32WindowTest, Basics) {
  TestWindow wnd;
  EXPECT_TRUE(wnd.handle() == NULL);
  EXPECT_FALSE(wnd.destroyed());
  EXPECT_TRUE(wnd.Create(0, L"Test", 0, 0, 0, 0, 100, 100));
  EXPECT_TRUE(wnd.handle() != NULL);
  EXPECT_EQ(kDummyResult, ::SendMessage(wnd.handle(), WM_USER, 1, 2));
  EXPECT_EQ(WM_USER, wnd.msg().message);
  EXPECT_EQ(1, wnd.msg().wParam);
  EXPECT_EQ(2, wnd.msg().lParam);
  wnd.Destroy();
  EXPECT_TRUE(wnd.handle() == NULL);
  EXPECT_TRUE(wnd.destroyed());
}

TEST(Win32WindowTest, MultipleWindows) {
  TestWindow wnd1, wnd2;
  EXPECT_TRUE(wnd1.Create(0, L"Test", 0, 0, 0, 0, 100, 100));
  EXPECT_TRUE(wnd2.Create(0, L"Test", 0, 0, 0, 0, 100, 100));
  EXPECT_TRUE(wnd1.handle() != NULL);
  EXPECT_TRUE(wnd2.handle() != NULL);
  wnd1.Destroy();
  wnd2.Destroy();
  EXPECT_TRUE(wnd2.handle() == NULL);
  EXPECT_TRUE(wnd1.handle() == NULL);
}
