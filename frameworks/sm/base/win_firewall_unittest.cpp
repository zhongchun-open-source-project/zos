// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/gunit.h"
#include "sm/base/win_firewall.h"

#include <objbase.h>

namespace sm {

TEST(WinFirewallTest, ReadStatus) {
  ::CoInitialize(NULL);
  WinFirewall fw;
  HRESULT hr;
  bool authorized;

  EXPECT_FALSE(fw.QueryAuthorized("bogus.exe", &authorized));
  EXPECT_TRUE(fw.Initialize(&hr));
  EXPECT_EQ(S_OK, hr);

  EXPECT_TRUE(fw.QueryAuthorized("bogus.exe", &authorized));

  // Unless we mock out INetFwMgr we can't really have an expectation either way
  // about whether we're authorized.  It will depend on the settings of the
  // machine running the test.  Same goes for AddApplication.

  fw.Shutdown();
  EXPECT_FALSE(fw.QueryAuthorized("bogus.exe", &authorized));

  ::CoUninitialize();
}

}  // namespace sm
