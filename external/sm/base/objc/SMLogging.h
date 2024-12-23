// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#import <Foundation/Foundation.h>

// Subset of LoggingSeverity.
typedef NS_ENUM(NSInteger, SMLoggingSeverity) {
  kSMLoggingSeverityVerbose,
  kSMLoggingSeverityInfo,
  kSMLoggingSeverityWarning,
  kSMLoggingSeverityError,
};

#if defined(__cplusplus)
extern "C" void SMLogEx(SMLoggingSeverity severity, NSString* log_string);
extern "C" void SMSetMinDebugLogLevel(SMLoggingSeverity severity);
extern "C" NSString* SMFileName(const char* filePath);
#else

// Wrapper for C++ LOG(sev) macros.
// Logs the log string to the sm logstream for the given severity.
extern void SMLogEx(SMLoggingSeverity severity, NSString* log_string);

// Wrapper for LogMessage::LogToDebug.
// Sets the minimum severity to be logged to console.
extern void SMSetMinDebugLogLevel(SMLoggingSeverity severity);

// Returns the filename with the path prefix removed.
extern NSString* SMFileName(const char* filePath);

#endif

// Some convenience macros.

#define SMLogString(format, ...)                    \
  [NSString stringWithFormat:@"(%@:%d %s): " format, \
      SMFileName(__FILE__),                         \
      __LINE__,                                      \
      __FUNCTION__,                                  \
      ##__VA_ARGS__]

#define SMLogFormat(severity, format, ...)                     \
  do {                                                          \
    NSString* log_string = SMLogString(format, ##__VA_ARGS__); \
    SMLogEx(severity, log_string);                             \
  } while (false)

#define SMLogVerbose(format, ...)                                \
  SMLogFormat(kSMLoggingSeverityVerbose, format, ##__VA_ARGS__) \

#define SMLogInfo(format, ...)                                   \
  SMLogFormat(kSMLoggingSeverityInfo, format, ##__VA_ARGS__)    \

#define SMLogWarning(format, ...)                                \
  SMLogFormat(kSMLoggingSeverityWarning, format, ##__VA_ARGS__) \

#define SMLogError(format, ...)                                  \
  SMLogFormat(kSMLoggingSeverityError, format, ##__VA_ARGS__)   \

#if !defined(NDEBUG)
#define SMLogDebug(format, ...) SMLogInfo(format, ##__VA_ARGS__)
#else
#define SMLogDebug(format, ...) \
  do {                           \
  } while (false)
#endif

#define SMLog(format, ...) SMLogInfo(format, ##__VA_ARGS__)
