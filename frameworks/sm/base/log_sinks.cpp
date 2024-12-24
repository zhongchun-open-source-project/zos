// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/log_sinks.h"

#include <iostream>
#include <string>

#include "sm/base/checks.h"

namespace sm {

FileRotatingLogSink::FileRotatingLogSink(const std::string& log_dir_path,
                                         const std::string& log_prefix,
                                         size_t max_log_size,
                                         size_t num_log_files)
    : FileRotatingLogSink(new FileRotatingStream(log_dir_path,
                                                 log_prefix,
                                                 max_log_size,
                                                 num_log_files)) {
}

FileRotatingLogSink::FileRotatingLogSink(FileRotatingStream* stream)
    : stream_(stream) {
  SM_DCHECK(stream);
}

FileRotatingLogSink::~FileRotatingLogSink() {
}

void FileRotatingLogSink::OnLogMessage(const std::string& message) {
  if (stream_->GetState() != SS_OPEN) {
    std::cerr << "Init() must be called before adding this sink." << std::endl;
    return;
  }
  stream_->WriteAll(message.c_str(), message.size(), nullptr, nullptr);
}

bool FileRotatingLogSink::Init() {
  return stream_->Open();
}

bool FileRotatingLogSink::DisableBuffering() {
  return stream_->DisableBuffering();
}

CallSessionFileRotatingLogSink::CallSessionFileRotatingLogSink(
    const std::string& log_dir_path,
    size_t max_total_log_size)
    : FileRotatingLogSink(
          new CallSessionFileRotatingStream(log_dir_path, max_total_log_size)) {
}

CallSessionFileRotatingLogSink::~CallSessionFileRotatingLogSink() {
}

}  // namespace sm
