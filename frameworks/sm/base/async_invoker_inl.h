// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_ASYNC_INVOKER_INL_H_
#define SM_BASE_ASYNC_INVOKER_INL_H_

#include "sm/base/bind.h"
#include "sm/base/callback.h"
#include "sm/base/critical_section.h"
#include "sm/base/message_handler.h"
#include "sm/base/refcount.h"
#include "sm/base/scoped_ref_ptr.h"
#include "sm/base/sigslot.h"
#include "sm/base/thread.h"

namespace sm {

class AsyncInvoker;

// Helper class for AsyncInvoker. Runs a task and triggers a callback
// on the calling thread if necessary. Instances are ref-counted so their
// lifetime can be independent of AsyncInvoker.
class AsyncClosure : public RefCountInterface {
 public:
  // Runs the asynchronous task, and triggers a callback to the calling
  // thread if needed. Should be called from the target thread.
  virtual void Execute() = 0;
 protected:
  ~AsyncClosure() override {}
};

// Simple closure that doesn't trigger a callback for the calling thread.
template <class FunctorT>
class FireAndForgetAsyncClosure : public AsyncClosure {
 public:
  explicit FireAndForgetAsyncClosure(const FunctorT& functor)
      : functor_(functor) {}
  virtual void Execute() {
    functor_();
  }
 private:
  FunctorT functor_;
};

// Base class for closures that may trigger a callback for the calling thread.
// Listens for the "destroyed" signals from the calling thread and the invoker,
// and cancels the callback to the calling thread if either is destroyed.
class NotifyingAsyncClosureBase : public AsyncClosure,
                                  public sigslot::has_slots<> {
 public:
  ~NotifyingAsyncClosureBase() override;

 protected:
  NotifyingAsyncClosureBase(AsyncInvoker* invoker, Thread* calling_thread);
  void TriggerCallback();
  void SetCallback(const Callback0<void>& callback) {
    CritScope cs(&crit_);
    callback_ = callback;
  }
  bool CallbackCanceled() const { return calling_thread_ == NULL; }

 private:
  Callback0<void> callback_;
  CriticalSection crit_;
  AsyncInvoker* invoker_;
  Thread* calling_thread_;

  void CancelCallback();
};

// Closures that have a non-void return value and require a callback.
template <class ReturnT, class FunctorT, class HostT>
class NotifyingAsyncClosure : public NotifyingAsyncClosureBase {
 public:
  NotifyingAsyncClosure(AsyncInvoker* invoker,
                        Thread* calling_thread,
                        const FunctorT& functor,
                        void (HostT::*callback)(ReturnT),
                        HostT* callback_host)
      :  NotifyingAsyncClosureBase(invoker, calling_thread),
         functor_(functor),
         callback_(callback),
         callback_host_(callback_host) {}
  virtual void Execute() {
    ReturnT result = functor_();
    if (!CallbackCanceled()) {
      SetCallback(Callback0<void>(Bind(callback_, callback_host_, result)));
      TriggerCallback();
    }
  }

 private:
  FunctorT functor_;
  void (HostT::*callback_)(ReturnT);
  HostT* callback_host_;
};

// Closures that have a void return value and require a callback.
template <class FunctorT, class HostT>
class NotifyingAsyncClosure<void, FunctorT, HostT>
    : public NotifyingAsyncClosureBase {
 public:
  NotifyingAsyncClosure(AsyncInvoker* invoker,
                        Thread* calling_thread,
                        const FunctorT& functor,
                        void (HostT::*callback)(),
                        HostT* callback_host)
      : NotifyingAsyncClosureBase(invoker, calling_thread),
        functor_(functor) {
    SetCallback(Callback0<void>(Bind(callback, callback_host)));
  }
  virtual void Execute() {
    functor_();
    TriggerCallback();
  }

 private:
  FunctorT functor_;
};

}  // namespace sm

#endif  // SM_BASE_ASYNC_INVOKER_INL_H_
