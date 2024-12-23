// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/gunit.h"
#include "sm/base/null_socket_server.h"

namespace sm {

static const uint32_t kTimeout = 5000U;

class NullSocketServerTest
    : public testing::Test,
      public MessageHandler {
 public:
  NullSocketServerTest() {}
 protected:
  virtual void OnMessage(Message* message) {
    ss_.WakeUp();
  }
  NullSocketServer ss_;
};

TEST_F(NullSocketServerTest, WaitAndSet) {
  Thread thread;
  EXPECT_TRUE(thread.Start());
  thread.Post(this, 0);
  // The process_io will be ignored.
  const bool process_io = true;
  EXPECT_TRUE_WAIT(ss_.Wait(SocketServer::kForever, process_io), kTimeout);
}

TEST_F(NullSocketServerTest, TestWait) {
  uint32_t start = Time();
  ss_.Wait(200, true);
  // The actual wait time is dependent on the resolution of the timer used by
  // the Event class. Allow for the event to signal ~20ms early.
  EXPECT_GE(TimeSince(start), 180);
}

}  // namespace sm
