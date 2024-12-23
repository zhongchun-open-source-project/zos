// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include <string>

#include "sm/base/gunit.h"
#include "sm/base/helpers.h"
#include "sm/base/logging.h"
#include "sm/base/path_utils.h"
#include "sm/base/scoped_ptr.h"
#include "sm/base/multipart.h"

namespace sm {

static const std::string kTestMultipartBoundary = "123456789987654321";
static const std::string kTestContentType =
    "multipart/form-data; boundary=123456789987654321";
static const char kTestData[] = "This is a test.";
static const char kTestStreamContent[] = "This is a test stream.";

TEST(MultipartTest, TestBasicOperations) {
  MultipartStream multipart("multipart/form-data", kTestMultipartBoundary);
  std::string content_type;
  multipart.GetContentType(&content_type);
  EXPECT_EQ(kTestContentType, content_type);

  EXPECT_EQ(SS_OPENING, multipart.GetState());

  // The multipart stream contains only --boundary--\r\n
  size_t end_part_size = multipart.GetEndPartSize();
  multipart.EndParts();
  EXPECT_EQ(SS_OPEN, multipart.GetState());
  size_t size;
  EXPECT_TRUE(multipart.GetSize(&size));
  EXPECT_EQ(end_part_size, size);

  // Write is not supported.
  EXPECT_EQ(SR_ERROR,
            multipart.Write(kTestData, sizeof(kTestData), NULL, NULL));

  multipart.Close();
  EXPECT_EQ(SS_CLOSED, multipart.GetState());
  EXPECT_TRUE(multipart.GetSize(&size));
  EXPECT_EQ(0U, size);
}

TEST(MultipartTest, TestAddAndRead) {
  MultipartStream multipart("multipart/form-data", kTestMultipartBoundary);

  size_t part_size =
      multipart.GetPartSize(kTestData, "form-data; name=\"text\"", "text");
  EXPECT_TRUE(multipart.AddPart(kTestData, "form-data; name=\"text\"", "text"));
  size_t size;
  EXPECT_TRUE(multipart.GetSize(&size));
  EXPECT_EQ(part_size, size);

  scoped_ptr<MemoryStream> stream(
      new MemoryStream(kTestStreamContent));
  size_t stream_size = 0;
  EXPECT_TRUE(stream->GetSize(&stream_size));
  part_size +=
      multipart.GetPartSize("", "form-data; name=\"stream\"", "stream");
  part_size += stream_size;

  EXPECT_TRUE(multipart.AddPart(
      new MemoryStream(kTestStreamContent),
      "form-data; name=\"stream\"",
      "stream"));
  EXPECT_TRUE(multipart.GetSize(&size));
  EXPECT_EQ(part_size, size);

  // In adding state, block read.
  char buffer[1024];
  EXPECT_EQ(SR_BLOCK,
            multipart.Read(buffer, sizeof(buffer), NULL, NULL));
  // Write is not supported.
  EXPECT_EQ(SR_ERROR,
            multipart.Write(buffer, sizeof(buffer), NULL, NULL));

  part_size += multipart.GetEndPartSize();
  multipart.EndParts();
  EXPECT_TRUE(multipart.GetSize(&size));
  EXPECT_EQ(part_size, size);

  // Read the multipart stream into StringStream
  std::string str;
  StringStream str_stream(&str);
  EXPECT_EQ(SR_SUCCESS,
            Flow(&multipart, buffer, sizeof(buffer), &str_stream));
  EXPECT_EQ(size, str.length());

  // Search three boundaries and two parts in the order.
  size_t pos = 0;
  pos = str.find(kTestMultipartBoundary);
  EXPECT_NE(std::string::npos, pos);
  pos += kTestMultipartBoundary.length();

  pos = str.find(kTestData, pos);
  EXPECT_NE(std::string::npos, pos);
  pos += sizeof(kTestData);

  pos = str.find(kTestMultipartBoundary, pos);
  EXPECT_NE(std::string::npos, pos);
  pos += kTestMultipartBoundary.length();

  pos = str.find(kTestStreamContent, pos);
  EXPECT_NE(std::string::npos, pos);
  pos += sizeof(kTestStreamContent);

  pos = str.find(kTestMultipartBoundary, pos);
  EXPECT_NE(std::string::npos, pos);
  pos += kTestMultipartBoundary.length();

  pos = str.find(kTestMultipartBoundary, pos);
  EXPECT_EQ(std::string::npos, pos);
}

}  // namespace sm
