// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/logcat_trace_context.h"

#include <android/log.h>
#include <assert.h>

#include "sm/base/logging.h"

namespace sm {

static android_LogPriority AndroidLogPriorityFromSmLogLevel(
    TraceLevel sm_level) {
  // NOTE: this mapping is somewhat arbitrary.  StateInfo and Info are mapped
  // to DEBUG because they are highly verbose in sm code (which is
  // unfortunate).
  switch (sm_level) {
    case kTraceStateInfo: return ANDROID_LOG_DEBUG;
    case kTraceWarning: return ANDROID_LOG_WARN;
    case kTraceError: return ANDROID_LOG_ERROR;
    case kTraceCritical: return ANDROID_LOG_FATAL;
    case kTraceApiCall: return ANDROID_LOG_VERBOSE;
    case kTraceModuleCall: return ANDROID_LOG_VERBOSE;
    case kTraceMemory: return ANDROID_LOG_VERBOSE;
    case kTraceTimer: return ANDROID_LOG_VERBOSE;
    case kTraceStream: return ANDROID_LOG_VERBOSE;
    case kTraceDebug: return ANDROID_LOG_DEBUG;
    case kTraceInfo: return ANDROID_LOG_DEBUG;
    case kTraceTerseInfo: return ANDROID_LOG_INFO;
    default:
      LOG(LS_ERROR) << "Unexpected log level" << sm_level;
      return ANDROID_LOG_FATAL;
  }
}

LogcatTraceContext::LogcatTraceContext() {
  Trace::CreateTrace();
  if (Trace::SetTraceCallback(this) != 0)
    assert(false);
}

LogcatTraceContext::~LogcatTraceContext() {
  if (Trace::SetTraceCallback(NULL) != 0)
    assert(false);
  Trace::ReturnTrace();
}

void LogcatTraceContext::Print(TraceLevel level,
                               const char* message,
                               int length) {
  __android_log_print(AndroidLogPriorityFromSmLogLevel(level),
                      "SM", "%.*s", length, message);
}

}  // namespace sm
