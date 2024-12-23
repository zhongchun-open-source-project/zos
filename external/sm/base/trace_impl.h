// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_TRACE_IMPL_H_
#define SM_BASE_TRACE_IMPL_H_

#include "sm/base/critical_section.h"
#include "sm/base/scoped_ptr.h"
#include "sm/base/event_wrapper.h"
#include "sm/base/file_wrapper.h"
#include "sm/base/static_instance.h"
#include "sm/base/platform_thread.h"
#include "sm/base/trace.h"

namespace sm {

#define SM_TRACE_MAX_MESSAGE_SIZE 1024
// Total buffer size is SM_TRACE_NUM_ARRAY (number of buffer partitions) *
// SM_TRACE_MAX_QUEUE (number of lines per buffer partition) *
// SM_TRACE_MAX_MESSAGE_SIZE (number of 1 byte charachters per line) =
// 1 or 4 Mbyte.

#define SM_TRACE_MAX_FILE_SIZE 100*1000
// Number of rows that may be written to file. On average 110 bytes per row (max
// 256 bytes per row). So on average 110*100*1000 = 11 Mbyte, max 256*100*1000 =
// 25.6 Mbyte

class TraceImpl : public Trace {
 public:
  virtual ~TraceImpl();

  static TraceImpl* CreateInstance();
  static TraceImpl* GetTrace(const TraceLevel level = kTraceAll);

  int32_t SetTraceFileImpl(const char* file_name, const bool add_file_counter);
  int32_t TraceFileImpl(char file_name[FileWrapper::kMaxFileNameSize]);

  int32_t SetTraceCallbackImpl(TraceCallback* callback);

  void AddImpl(const TraceLevel level, const TraceModule module,
               const int32_t id, const char* msg);

  bool TraceCheck(const TraceLevel level) const;

 protected:
  TraceImpl();

  static TraceImpl* StaticInstance(CountOperation count_operation,
                                   const TraceLevel level = kTraceAll);

  int32_t AddThreadId(char* trace_message) const;

  // OS specific implementations.
  virtual int32_t AddTime(char* trace_message,
                          const TraceLevel level) const = 0;

  virtual int32_t AddDateTimeInfo(char* trace_message) const = 0;

 private:
  friend class Trace;

  int32_t AddLevel(char* sz_message, const TraceLevel level) const;

  int32_t AddModuleAndId(char* trace_message, const TraceModule module,
                         const int32_t id) const;

  int32_t AddMessage(char* trace_message,
                     const char msg[SM_TRACE_MAX_MESSAGE_SIZE],
                     const uint16_t written_so_far) const;

  void AddMessageToList(
    const char trace_message[SM_TRACE_MAX_MESSAGE_SIZE],
    const uint16_t length,
    const TraceLevel level);

  bool UpdateFileName(
    const char file_name_utf8[FileWrapper::kMaxFileNameSize],
    char file_name_with_counter_utf8[FileWrapper::kMaxFileNameSize],
    const uint32_t new_count) const;

  bool CreateFileName(
    const char file_name_utf8[FileWrapper::kMaxFileNameSize],
    char file_name_with_counter_utf8[FileWrapper::kMaxFileNameSize],
    const uint32_t new_count) const;

  void WriteToFile(const char* msg, uint16_t length)
      EXCLUSIVE_LOCKS_REQUIRED(crit_);

  TraceCallback* callback_ GUARDED_BY(crit_);
  uint32_t row_count_text_ GUARDED_BY(crit_);
  uint32_t file_count_text_ GUARDED_BY(crit_);

  const scoped_ptr<FileWrapper> trace_file_ GUARDED_BY(crit_);
  CriticalSection crit_;
};

}  // namespace sm

#endif  // SM_BASE_TRACE_IMPL_H_
