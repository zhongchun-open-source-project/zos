// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/critical_section_win.h"

namespace sm {

CriticalSectionWindows::CriticalSectionWindows() {
  InitializeCriticalSection(&crit);
}

CriticalSectionWindows::~CriticalSectionWindows() {
  DeleteCriticalSection(&crit);
}

void
CriticalSectionWindows::Enter() {
  EnterCriticalSection(&crit);
}

void
CriticalSectionWindows::Leave() {
  LeaveCriticalSection(&crit);
}

}  // namespace sm
