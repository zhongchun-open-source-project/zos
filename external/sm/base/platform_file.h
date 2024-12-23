// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_PLATFORM_FILE_H_
#define SM_BASE_PLATFORM_FILE_H_

#include <stdio.h>

#if defined(SM_WIN)
#include <windows.h>
#endif

namespace sm {

#if defined(SM_WIN)
typedef HANDLE PlatformFile;
#elif defined(SM_POSIX)
typedef int PlatformFile;
#else
#error Unsupported platform
#endif

extern const PlatformFile kInvalidPlatformFileValue;

// Associates a standard FILE stream with an existing PlatformFile.
// Note that after this function has returned a valid FILE stream,
// the PlatformFile should no longer be used.
FILE* FdopenPlatformFileForWriting(PlatformFile file);

// Closes a PlatformFile.
// Don't use ClosePlatformFile to close a file opened with FdopenPlatformFile.
// Use fclose instead.
bool ClosePlatformFile(PlatformFile file);

}  // namespace sm

#endif  // SM_BASE_PLATFORM_FILE_H_
