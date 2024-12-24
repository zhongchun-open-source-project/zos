// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_NULL_SOCKET_SERVER_H_
#define SM_BASE_NULL_SOCKET_SERVER_H_

#include "sm/base/event.h"
#include "sm/base/physical_socket_server.h"

namespace sm {

// NullSocketServer

class NullSocketServer : public SocketServer {
 public:
  NullSocketServer() : event_(false, false) {}

  virtual bool Wait(int cms, bool process_io) {
    event_.Wait(cms);
    return true;
  }

  virtual void WakeUp() {
    event_.Set();
  }

  virtual Socket* CreateSocket(int type) {
    ASSERT(false);
    return NULL;
  }

  virtual Socket* CreateSocket(int family, int type) {
    ASSERT(false);
    return NULL;
  }

  virtual AsyncSocket* CreateAsyncSocket(int type) {
    ASSERT(false);
    return NULL;
  }

  virtual AsyncSocket* CreateAsyncSocket(int family, int type) {
    ASSERT(false);
    return NULL;
  }


 private:
  Event event_;
};

}  // namespace sm

#endif  // SM_BASE_NULL_SOCKET_SERVER_H_
