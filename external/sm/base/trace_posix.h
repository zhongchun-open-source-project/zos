// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_TRACE_POSIX_H_
#define SM_BASE_TRACE_POSIX_H_

#include "sm/base/critical_section_wrapper.h"
#include "sm/base/trace_impl.h"

namespace sm {

class TracePosix : public TraceImpl {
 public:
  TracePosix();
  ~TracePosix() override;

  // This method can be called on several different threads different from
  // the creating thread.
  int32_t AddTime(char* trace_message, const TraceLevel level) const override;

  int32_t AddDateTimeInfo(char* trace_message) const override;

 private:
  volatile mutable uint32_t  prev_api_tick_count_;
  volatile mutable uint32_t  prev_tick_count_;

  CriticalSectionWrapper& crit_sect_;
};

}  // namespace sm

#endif  // SM_BASE_TRACE_POSIX_H_
