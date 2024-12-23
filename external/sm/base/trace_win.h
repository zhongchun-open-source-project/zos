// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_TRACE_WIN_H_
#define SM_BASE_TRACE_WIN_H_

#include <stdio.h>
#include <windows.h>

#include "sm/base/trace_impl.h"

namespace sm {

class TraceWindows : public TraceImpl {
 public:
  TraceWindows();
  virtual ~TraceWindows();

  virtual int32_t AddTime(char* trace_message, const TraceLevel level) const;

  virtual int32_t AddDateTimeInfo(char* trace_message) const;
 private:
  volatile mutable uint32_t prev_api_tick_count_;
  volatile mutable uint32_t prev_tick_count_;
};

}  // namespace sm

#endif  // SM_BASE_TRACE_WIN_H_
