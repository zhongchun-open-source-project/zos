// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_PLATFORM_THREAD_TYPES_H_
#define SM_BASE_PLATFORM_THREAD_TYPES_H_

#if defined(SM_WIN)
#include <winsock2.h>
#include <windows.h>
#elif defined(SM_POSIX)
#include <pthread.h>
#include <unistd.h>
#endif

namespace sm {
#if defined(SM_WIN)
typedef DWORD PlatformThreadId;
typedef DWORD PlatformThreadRef;
#elif defined(SM_POSIX)
typedef pid_t PlatformThreadId;
typedef pthread_t PlatformThreadRef;
#endif
}  // namespace sm

#endif  // SM_BASE_PLATFORM_THREAD_TYPES_H_
