// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/win32_socket_init.h"

#include "sm/base/win32.h"

namespace sm {

// Please don't remove this function.
void EnsureWinsockInit() {
  // The default implementation uses a global initializer, so WSAStartup
  // happens at module load time.  Thus we don't need to do anything here.
  // The hook is provided so that a client that statically links with
  // libsm can override it, to provide its own initialization.
}

#if defined(SM_WIN)
class WinsockInitializer {
 public:
  WinsockInitializer() {
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(1, 0);
    err_ = WSAStartup(wVersionRequested, &wsaData);
  }
  ~WinsockInitializer() {
    if (!err_)
      WSACleanup();
  }
  int error() {
    return err_;
  }
 private:
  int err_;
};
WinsockInitializer g_winsockinit;
#endif

}  // namespace sm
