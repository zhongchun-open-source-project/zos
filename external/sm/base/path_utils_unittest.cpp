// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/path_utils.h"
#include "sm/base/gunit.h"

TEST(Pathname, ReturnsDotForEmptyPathname) {
  const std::string kCWD =
      std::string(".") + Pathname::DefaultFolderDelimiter();

  Pathname path("/", "");
  EXPECT_FALSE(path.empty());
  EXPECT_FALSE(path.folder().empty());
  EXPECT_TRUE (path.filename().empty());
  EXPECT_FALSE(path.pathname().empty());
  EXPECT_EQ(std::string("/"), path.pathname());

  path.SetPathname("", "foo");
  EXPECT_FALSE(path.empty());
  EXPECT_TRUE (path.folder().empty());
  EXPECT_FALSE(path.filename().empty());
  EXPECT_FALSE(path.pathname().empty());
  EXPECT_EQ(std::string("foo"), path.pathname());

  path.SetPathname("", "");
  EXPECT_TRUE (path.empty());
  EXPECT_TRUE (path.folder().empty());
  EXPECT_TRUE (path.filename().empty());
  EXPECT_FALSE(path.pathname().empty());
  EXPECT_EQ(kCWD, path.pathname());

  path.SetPathname(kCWD, "");
  EXPECT_FALSE(path.empty());
  EXPECT_FALSE(path.folder().empty());
  EXPECT_TRUE (path.filename().empty());
  EXPECT_FALSE(path.pathname().empty());
  EXPECT_EQ(kCWD, path.pathname());

  Pathname path2("c:/foo bar.txt");
  EXPECT_EQ(path2.url(), std::string("file:///c:/foo%20bar.txt"));
}
