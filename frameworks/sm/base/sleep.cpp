// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

// An OS-independent sleep function.

#include "sm/base/sleep.h"

#ifdef _WIN32
// For Sleep()
#include <windows.h>
#else
// For nanosleep()
#include <time.h>
#endif

namespace sm {

void SleepMs(int msecs) {
#ifdef _WIN32
  Sleep(msecs);
#else
  struct timespec short_wait;
  struct timespec remainder;
  short_wait.tv_sec = msecs / 1000;
  short_wait.tv_nsec = (msecs % 1000) * 1000 * 1000;
  nanosleep(&short_wait, &remainder);
#endif
}

}  // namespace sm
