// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/logging.h"

#include <string.h>

#include <sstream>

#include "sm/common_types.h"
#include "sm/base/trace.h"

namespace sm {
namespace {

TraceLevel SmSeverity(LoggingSeverity sev) {
  switch (sev) {
    // TODO(ajm): SENSITIVE doesn't have a corresponding sm level.
    case LS_SENSITIVE:  return kTraceInfo;
    case LS_VERBOSE:    return kTraceInfo;
    case LS_INFO:       return kTraceTerseInfo;
    case LS_WARNING:    return kTraceWarning;
    case LS_ERROR:      return kTraceError;
    default:            return kTraceNone;
  }
}

// Return the filename portion of the string (that following the last slash).
const char* FilenameFromPath(const char* file) {
  const char* end1 = ::strrchr(file, '/');
  const char* end2 = ::strrchr(file, '\\');
  if (!end1 && !end2)
    return file;
  else
    return (end1 > end2) ? end1 + 1 : end2 + 1;
}

}  // namespace

LogMessage::LogMessage(const char* file, int line, LoggingSeverity sev)
    : severity_(sev) {
  print_stream_ << "(" << FilenameFromPath(file) << ":" << line << "): ";
}

bool LogMessage::Loggable(LoggingSeverity sev) {
  // |level_filter| is a bitmask, unlike libsm's minimum severity value.
  return SmSeverity(sev) & Trace::level_filter() ? true : false;
}

LogMessage::~LogMessage() {
  const std::string& str = print_stream_.str();
  Trace::Add(SmSeverity(severity_), kTraceUndefined, 0, "%s", str.c_str());
}

}  // namespace sm
