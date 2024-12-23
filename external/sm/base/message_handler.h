// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_MESSAGE_HANDLER_H_
#define SM_BASE_MESSAGE_HANDLER_H_

#include <utility>

#include "sm/base/constructor_magic.h"
#include "sm/base/scoped_ptr.h"

namespace sm {

struct Message;

// Messages get dispatched to a MessageHandler

class MessageHandler {
 public:
  virtual ~MessageHandler();
  virtual void OnMessage(Message* msg) = 0;

 protected:
  MessageHandler() {}

 private:
  SM_DISALLOW_COPY_AND_ASSIGN(MessageHandler);
};

// Helper class to facilitate executing a functor on a thread.
template <class ReturnT, class FunctorT>
class FunctorMessageHandler : public MessageHandler {
 public:
  explicit FunctorMessageHandler(const FunctorT& functor)
      : functor_(functor) {}
  virtual void OnMessage(Message* msg) {
    result_ = functor_();
  }
  const ReturnT& result() const { return result_; }

 private:
  FunctorT functor_;
  ReturnT result_;
};

// Specialization for scoped_ptr<ReturnT>.
template <class ReturnT, class FunctorT>
class FunctorMessageHandler<class scoped_ptr<ReturnT>, FunctorT>
    : public MessageHandler {
 public:
  explicit FunctorMessageHandler(const FunctorT& functor) : functor_(functor) {}
  virtual void OnMessage(Message* msg) { result_ = std::move(functor_()); }
  scoped_ptr<ReturnT> result() { return std::move(result_); }

 private:
  FunctorT functor_;
  scoped_ptr<ReturnT> result_;
};

// Specialization for ReturnT of void.
template <class FunctorT>
class FunctorMessageHandler<void, FunctorT> : public MessageHandler {
 public:
  explicit FunctorMessageHandler(const FunctorT& functor)
      : functor_(functor) {}
  virtual void OnMessage(Message* msg) {
    functor_();
  }
  void result() const {}

 private:
  FunctorT functor_;
};

} // namespace sm

#endif // SM_BASE_MESSAGE_HANDLER_H_
