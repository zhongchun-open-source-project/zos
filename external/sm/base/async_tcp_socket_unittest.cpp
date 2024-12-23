// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include <string>

#include "sm/base/async_tcp_socket.h"
#include "sm/base/gunit.h"
#include "sm/base/physical_socket_server.h"
#include "sm/base/scoped_ptr.h"
#include "sm/base/virtual_socket_server.h"

namespace sm {

class AsyncTCPSocketTest
    : public testing::Test,
      public sigslot::has_slots<> {
 public:
  AsyncTCPSocketTest()
      : pss_(new PhysicalSocketServer),
        vss_(new VirtualSocketServer(pss_.get())),
        socket_(vss_->CreateAsyncSocket(SOCK_STREAM)),
        tcp_socket_(new AsyncTCPSocket(socket_, true)),
        ready_to_send_(false) {
    tcp_socket_->SignalReadyToSend.connect(this,
                                           &AsyncTCPSocketTest::OnReadyToSend);
  }

  void OnReadyToSend(AsyncPacketSocket* socket) {
    ready_to_send_ = true;
  }

 protected:
  scoped_ptr<PhysicalSocketServer> pss_;
  scoped_ptr<VirtualSocketServer> vss_;
  AsyncSocket* socket_;
  scoped_ptr<AsyncTCPSocket> tcp_socket_;
  bool ready_to_send_;
};

TEST_F(AsyncTCPSocketTest, OnWriteEvent) {
  EXPECT_FALSE(ready_to_send_);
  socket_->SignalWriteEvent(socket_);
  EXPECT_TRUE(ready_to_send_);
}

}  // namespace sm
