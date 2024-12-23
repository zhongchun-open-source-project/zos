// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_FILE_IMPL_H_
#define SM_BASE_FILE_IMPL_H_

#include <stdio.h>

#include "sm/base/scoped_ptr.h"
#include "sm/base/file_wrapper.h"

namespace sm {

class RWLockWrapper;

class FileWrapperImpl : public FileWrapper {
 public:
  FileWrapperImpl();
  ~FileWrapperImpl() override;

  int FileName(char* file_name_utf8, size_t size) const override;

  bool Open() const override;

  int OpenFile(const char* file_name_utf8,
               bool read_only,
               bool loop = false,
               bool text = false) override;

  int OpenFromFileHandle(FILE* handle,
                         bool manage_file,
                         bool read_only,
                         bool loop = false) override;

  int CloseFile() override;
  int SetMaxFileSize(size_t bytes) override;
  int Flush() override;

  int Read(void* buf, size_t length) override;
  bool Write(const void* buf, size_t length) override;
  int WriteText(const char* format, ...) override;
  int Rewind() override;

 private:
  int CloseFileImpl();
  int FlushImpl();

  scoped_ptr<RWLockWrapper> rw_lock_;

  FILE* id_;
  bool managed_file_handle_;
  bool open_;
  bool looping_;
  bool read_only_;
  size_t max_size_in_bytes_;  // -1 indicates file size limitation is off
  size_t size_in_bytes_;
  char file_name_utf8_[kMaxFileNameSize];
};

}  // namespace sm

#endif  // SM_BASE_FILE_IMPL_H_
