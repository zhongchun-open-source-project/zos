// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#if defined(_WIN32)
#include <windows.h>
#include "sm/base/critical_section_win.h"
#else
#include "sm/base/critical_section_posix.h"
#endif

namespace sm {

CriticalSectionWrapper* CriticalSectionWrapper::CreateCriticalSection() {
#ifdef _WIN32
  return new CriticalSectionWindows();
#else
  return new CriticalSectionPosix();
#endif
}

}  // namespace sm
