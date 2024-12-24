// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

// A libsm compatible SocketServer for OSX/iOS/Cocoa.

#ifndef SM_BASE_MAC_COCOA_SOCKET_SERVER_H_
#define SM_BASE_MAC_COCOA_SOCKET_SERVER_H_

#include "sm/base/mac_socket_server.h"

#ifdef __OBJC__
@class NSTimer, MacCocoaSocketServerHelperSm;
#else
class NSTimer;
class MacCocoaSocketServerHelperSm;
#endif

namespace sm {

// A socketserver implementation that wraps the main cocoa
// application loop accessed through [NSApp run].
class MacCocoaSocketServer : public MacBaseSocketServer {
 public:
  explicit MacCocoaSocketServer();
  ~MacCocoaSocketServer() override;

  bool Wait(int cms, bool process_io) override;
  void WakeUp() override;

 private:
  MacCocoaSocketServerHelperSm* helper_;
  NSTimer* timer_;  // Weak.
  // The count of how many times we're inside the NSApplication main loop.
  int run_count_;

  SM_DISALLOW_COPY_AND_ASSIGN(MacCocoaSocketServer);
};

}  // namespace sm

#endif  // SM_BASE_MAC_COCOA_SOCKET_SERVER_H_
