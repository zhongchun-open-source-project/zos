// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/platform_file.h"

#if defined(SM_WIN)
#include <io.h>
#else
#include <unistd.h>
#endif

namespace sm {

#if defined(SM_WIN)
const PlatformFile kInvalidPlatformFileValue = INVALID_HANDLE_VALUE;

FILE* FdopenPlatformFileForWriting(PlatformFile file) {
  if (file == kInvalidPlatformFileValue)
    return NULL;
  int fd = _open_osfhandle(reinterpret_cast<intptr_t>(file), 0);
  if (fd < 0)
    return NULL;

  return _fdopen(fd, "w");
}

bool ClosePlatformFile(PlatformFile file) {
  return CloseHandle(file) != 0;
}
#else
const PlatformFile kInvalidPlatformFileValue = -1;

FILE* FdopenPlatformFileForWriting(PlatformFile file) {
  return fdopen(file, "w");
}

bool ClosePlatformFile(PlatformFile file) {
  return close(file);
}
#endif

}  // namespace sm
