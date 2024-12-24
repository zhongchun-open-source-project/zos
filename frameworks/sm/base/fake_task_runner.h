// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

// A fake TaskRunner for use in unit tests.

#ifndef SM_BASE_FAKE_TASK_RUNNER_H_
#define SM_BASE_FAKE_TASK_RUNNER_H_

#include "sm/base/task_parent.h"
#include "sm/base/task_runner.h"

namespace sm {

class FakeTaskRunner : public TaskRunner {
 public:
  FakeTaskRunner() : current_time_(0) {}
  virtual ~FakeTaskRunner() {}

  virtual void WakeTasks() { RunTasks(); }

  virtual int64_t CurrentTime() {
    // Implement if needed.
    return current_time_++;
  }

  int64_t current_time_;
};

}  // namespace sm

#endif  // SM_BASE_FAKE_TASK_RUNNER_H_
