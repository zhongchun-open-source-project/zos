// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_FILE_ROTATING_LOG_SINK_H_
#define SM_BASE_FILE_ROTATING_LOG_SINK_H_

#include <string>

#include "sm/base/constructor_magic.h"
#include "sm/base/file_rotating_stream.h"
#include "sm/base/logging.h"
#include "sm/base/scoped_ptr.h"

namespace sm {

// Log sink that uses a FileRotatingStream to write to disk.
// Init() must be called before adding this sink.
class FileRotatingLogSink : public LogSink {
 public:
  // |num_log_files| must be greater than 1 and |max_log_size| must be greater
  // than 0.
  FileRotatingLogSink(const std::string& log_dir_path,
                      const std::string& log_prefix,
                      size_t max_log_size,
                      size_t num_log_files);
  ~FileRotatingLogSink() override;

  // Writes the message to the current file. It will spill over to the next
  // file if needed.
  void OnLogMessage(const std::string& message) override;

  // Deletes any existing files in the directory and creates a new log file.
  virtual bool Init();

  // Disables buffering on the underlying stream.
  bool DisableBuffering();

 protected:
  explicit FileRotatingLogSink(FileRotatingStream* stream);

 private:
  scoped_ptr<FileRotatingStream> stream_;

  SM_DISALLOW_COPY_AND_ASSIGN(FileRotatingLogSink);
};

// Log sink that uses a CallSessionFileRotatingStream to write to disk.
// Init() must be called before adding this sink.
class CallSessionFileRotatingLogSink : public FileRotatingLogSink {
 public:
  CallSessionFileRotatingLogSink(const std::string& log_dir_path,
                                 size_t max_total_log_size);
  ~CallSessionFileRotatingLogSink() override;

 private:
  SM_DISALLOW_COPY_AND_ASSIGN(CallSessionFileRotatingLogSink);
};

}  // namespace sm

#endif  // SM_BASE_FILE_ROTATING_LOG_SINK_H_
