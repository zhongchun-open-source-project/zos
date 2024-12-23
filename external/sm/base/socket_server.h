// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_SOCKET_SERVER_H_
#define SM_BASE_SOCKET_SERVER_H_

#include "sm/base/socket_factory.h"

namespace sm {

class MessageQueue;

// Provides the ability to wait for activity on a set of sockets.  The Thread
// class provides a nice wrapper on a socket server.
//
// The server is also a socket factory.  The sockets it creates will be
// notified of asynchronous I/O from this server's Wait method.
class SocketServer : public SocketFactory {
 public:
  static const int kForever = -1;

  // When the socket server is installed into a Thread, this function is
  // called to allow the socket server to use the thread's message queue for
  // any messaging that it might need to perform.
  virtual void SetMessageQueue(MessageQueue* queue) {}

  // Sleeps until:
  //  1) cms milliseconds have elapsed (unless cms == kForever)
  //  2) WakeUp() is called
  // While sleeping, I/O is performed if process_io is true.
  virtual bool Wait(int cms, bool process_io) = 0;

  // Causes the current wait (if one is in progress) to wake up.
  virtual void WakeUp() = 0;
};

}  // namespace sm

#endif  // SM_BASE_SOCKET_SERVER_H_
