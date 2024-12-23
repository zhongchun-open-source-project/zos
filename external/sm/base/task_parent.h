// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_TASK_PARENT_H_
#define SM_BASE_TASK_PARENT_H_

#include <set>

#include "sm/base/basic_types.h"
#include "sm/base/scoped_ptr.h"

namespace sm {

class Task;
class TaskRunner;

class TaskParent {
 public:
  TaskParent(Task *derived_instance, TaskParent *parent);
  explicit TaskParent(TaskRunner *derived_instance);
  virtual ~TaskParent();

  TaskParent *GetParent() { return parent_; }
  TaskRunner *GetRunner() { return runner_; }

  bool AllChildrenDone();
  bool AnyChildError();
#if !defined(NDEBUG)
  bool IsChildTask(Task *task);
#endif

 protected:
  void OnStopped(Task *task);
  void AbortAllChildren();
  TaskParent *parent() {
    return parent_;
  }

 private:
  void Initialize();
  void OnChildStopped(Task *child);
  void AddChild(Task *child);

  TaskParent *parent_;
  TaskRunner *runner_;
  bool child_error_;
  typedef std::set<Task *> ChildSet;
  scoped_ptr<ChildSet> children_;
  SM_DISALLOW_COPY_AND_ASSIGN(TaskParent);
};


} // namespace sm

#endif  // SM_BASE_TASK_PARENT_H_
