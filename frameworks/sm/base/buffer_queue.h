// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_BUFFER_QUEUE_H_
#define SM_BASE_BUFFER_QUEUE_H_

#include <deque>
#include <vector>

#include "sm/base/buffer.h"
#include "sm/base/critical_section.h"

namespace sm {

class BufferQueue {
 public:
  // Creates a buffer queue with a given capacity and default buffer size.
  BufferQueue(size_t capacity, size_t default_size);
  virtual ~BufferQueue();

  // Return number of queued buffers.
  size_t size() const;

  // ReadFront will only read one buffer at a time and will truncate buffers
  // that don't fit in the passed memory.
  // Returns true unless no data could be returned.
  bool ReadFront(void* data, size_t bytes, size_t* bytes_read);

  // WriteBack always writes either the complete memory or nothing.
  // Returns true unless no data could be written.
  bool WriteBack(const void* data, size_t bytes, size_t* bytes_written);

 protected:
  // These methods are called when the state of the queue changes.
  virtual void NotifyReadableForTest() {}
  virtual void NotifyWritableForTest() {}

 private:
  size_t capacity_;
  size_t default_size_;
  mutable CriticalSection crit_;
  std::deque<Buffer*> queue_ GUARDED_BY(crit_);
  std::vector<Buffer*> free_list_ GUARDED_BY(crit_);

  SM_DISALLOW_COPY_AND_ASSIGN(BufferQueue);
};

}  // namespace sm

#endif  // SM_BASE_BUFFER_QUEUE_H_
