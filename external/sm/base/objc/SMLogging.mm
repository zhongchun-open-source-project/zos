// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#import "SMLogging.h"

#include "sm/base/logging.h"

LoggingSeverity SMGetNativeLoggingSeverity(SMLoggingSeverity severity) {
  switch (severity) {
    case kSMLoggingSeverityVerbose:
      return LS_VERBOSE;
    case kSMLoggingSeverityInfo:
      return LS_INFO;
    case kSMLoggingSeverityWarning:
      return LS_WARNING;
    case kSMLoggingSeverityError:
      return LS_ERROR;
  }
}

void SMLogEx(SMLoggingSeverity severity, NSString* log_string) {
  if (log_string.length) {
    const char* utf8_string = log_string.UTF8String;
    LOG_V(SMGetNativeLoggingSeverity(severity)) << utf8_string;
  }
}

void SMSetMinDebugLogLevel(SMLoggingSeverity severity) {
  LogMessage::LogToDebug(SMGetNativeLoggingSeverity(severity));
}

NSString* SMFileName(const char* file_path) {
  NSString* ns_file_path =
      [[NSString alloc] initWithBytesNoCopy:const_cast<char*>(file_path)
                                     length:strlen(file_path)
                                   encoding:NSUTF8StringEncoding
                               freeWhenDone:NO];
  return ns_file_path.lastPathComponent;
}

