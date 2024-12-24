// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/array_size.h"
#include "sm/base/common.h"
#include "sm/base/gunit.h"
#include "sm/base/thread.h"
#include "sm/base/url_encode.h"

using UrlEncode;

TEST(Urlencode, SourceTooLong) {
  char source[] = "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
      "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^";
  char dest[1];
  ASSERT_EQ(0, UrlEncode(source, dest, arraysize(dest)));
  ASSERT_EQ('\0', dest[0]);

  dest[0] = 'a';
  ASSERT_EQ(0, UrlEncode(source, dest, 0));
  ASSERT_EQ('a', dest[0]);
}

TEST(Urlencode, OneCharacterConversion) {
  char source[] = "^";
  char dest[4];
  ASSERT_EQ(3, UrlEncode(source, dest, arraysize(dest)));
  ASSERT_STREQ("%5E", dest);
}

TEST(Urlencode, ShortDestinationNoEncoding) {
  // In this case we have a destination that would not be
  // big enough to hold an encoding but is big enough to
  // hold the text given.
  char source[] = "aa";
  char dest[3];
  ASSERT_EQ(2, UrlEncode(source, dest, arraysize(dest)));
  ASSERT_STREQ("aa", dest);
}

TEST(Urlencode, ShortDestinationEncoding) {
  // In this case we have a destination that is not
  // big enough to hold the encoding.
  char source[] = "&";
  char dest[3];
  ASSERT_EQ(0, UrlEncode(source, dest, arraysize(dest)));
  ASSERT_EQ('\0', dest[0]);
}

TEST(Urlencode, Encoding1) {
  char source[] = "A^ ";
  char dest[8];
  ASSERT_EQ(5, UrlEncode(source, dest, arraysize(dest)));
  ASSERT_STREQ("A%5E+", dest);
}

TEST(Urlencode, Encoding2) {
  char source[] = "A^ ";
  char dest[8];
  ASSERT_EQ(7, UrlEncodeWithoutEncodingSpaceAsPlus(source, dest,
                                                        arraysize(dest)));
  ASSERT_STREQ("A%5E%20", dest);
}

TEST(Urldecode, Decoding1) {
  char source[] = "A%5E+";
  char dest[8];
  ASSERT_EQ(3, UrlDecode(source, dest));
  ASSERT_STREQ("A^ ", dest);
}

TEST(Urldecode, Decoding2) {
  char source[] = "A%5E+";
  char dest[8];
  ASSERT_EQ(3, UrlDecodeWithoutEncodingSpaceAsPlus(source, dest));
  ASSERT_STREQ("A^+", dest);
}
