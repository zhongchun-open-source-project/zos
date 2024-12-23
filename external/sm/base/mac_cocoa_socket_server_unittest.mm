// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/gunit.h"
#include "sm/base/scoped_ptr.h"
#include "sm/base/thread.h"
#include "sm/base/mac_cocoa_socket_server.h"

namespace sm {

class WakeThread : public Thread {
 public:
  WakeThread(SocketServer* ss) : ss_(ss) {
  }
  virtual ~WakeThread() {
    Stop();
  }
  void Run() {
    ss_->WakeUp();
  }
 private:
  SocketServer* ss_;
};

// Test that MacCocoaSocketServer::Wait works as expected.
TEST(MacCocoaSocketServer, TestWait) {
  MacCocoaSocketServer server;
  uint32_t start = Time();
  server.Wait(1000, true);
  EXPECT_GE(TimeSince(start), 1000);
}

// Test that MacCocoaSocketServer::Wakeup works as expected.
TEST(MacCocoaSocketServer, TestWakeup) {
  MacCFSocketServer server;
  WakeThread thread(&server);
  uint32_t start = Time();
  thread.Start();
  server.Wait(10000, true);
  EXPECT_LT(TimeSince(start), 10000);
}

} // namespace sm
