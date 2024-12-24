// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_SOCKET_FACTORY_H_
#define SM_BASE_SOCKET_FACTORY_H_

#include "sm/base/socket.h"
#include "sm/base/async_socket.h"

namespace sm {

class SocketFactory {
public:
  virtual ~SocketFactory() {}

  // Returns a new socket for blocking communication.  The type can be
  // SOCK_DGRAM and SOCK_STREAM.
  // TODO: C++ inheritance rules mean that all users must have both
  // CreateSocket(int) and CreateSocket(int,int). Will remove CreateSocket(int)
  // (and CreateAsyncSocket(int) when all callers are changed.
  virtual Socket* CreateSocket(int type) = 0;
  virtual Socket* CreateSocket(int family, int type) = 0;
  // Returns a new socket for nonblocking communication.  The type can be
  // SOCK_DGRAM and SOCK_STREAM.
  virtual AsyncSocket* CreateAsyncSocket(int type) = 0;
  virtual AsyncSocket* CreateAsyncSocket(int family, int type) = 0;
};

} // namespace sm

#endif // SM_BASE_SOCKET_FACTORY_H_
