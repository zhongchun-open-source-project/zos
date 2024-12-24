// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/worker.h"

#include "sm/base/common.h"
#include "sm/base/logging.h"
#include "sm/base/thread.h"

namespace sm {

enum {
  MSG_HAVEWORK = 0,
};

Worker::Worker() : worker_thread_(NULL) {}

Worker::~Worker() {
  // We need to already be stopped before being destroyed. We cannot call
  // StopWork() from here because the subclass's data has already been
  // destructed, so OnStop() cannot be called.
  ASSERT(!worker_thread_);
}

bool Worker::StartWork() {
  Thread *me = Thread::Current();
  if (worker_thread_) {
    if (worker_thread_ == me) {
      // Already working on this thread, so nothing to do.
      return true;
    } else {
      LOG(LS_ERROR) << "Automatically switching threads is not supported";
      ASSERT(false);
      return false;
    }
  }
  worker_thread_ = me;
  OnStart();
  return true;
}

bool Worker::StopWork() {
  if (!worker_thread_) {
    // Already not working, so nothing to do.
    return true;
  } else if (worker_thread_ != Thread::Current()) {
    LOG(LS_ERROR) << "Stopping from a different thread is not supported";
    ASSERT(false);
    return false;
  }
  OnStop();
  worker_thread_->Clear(this, MSG_HAVEWORK);
  worker_thread_ = NULL;
  return true;
}

void Worker::HaveWork() {
  ASSERT(worker_thread_ != NULL);
  worker_thread_->Post(this, MSG_HAVEWORK);
}

void Worker::OnMessage(Message *msg) {
  ASSERT(msg->message_id == MSG_HAVEWORK);
  ASSERT(worker_thread_ == Thread::Current());
  OnHaveWork();
}

}  // namespace sm
