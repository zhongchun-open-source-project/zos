// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include <string>

#include "sm/base/async_udp_socket.h"
#include "sm/base/gunit.h"
#include "sm/base/physical_socket_server.h"
#include "sm/base/scoped_ptr.h"
#include "sm/base/virtual_socket_server.h"

namespace sm {

class AsyncUdpSocketTest
    : public testing::Test,
      public sigslot::has_slots<> {
 public:
  AsyncUdpSocketTest()
      : pss_(new PhysicalSocketServer),
        vss_(new VirtualSocketServer(pss_.get())),
        socket_(vss_->CreateAsyncSocket(SOCK_DGRAM)),
        udp_socket_(new AsyncUDPSocket(socket_)),
        ready_to_send_(false) {
    udp_socket_->SignalReadyToSend.connect(this,
                                           &AsyncUdpSocketTest::OnReadyToSend);
  }

  void OnReadyToSend(AsyncPacketSocket* socket) {
    ready_to_send_ = true;
  }

 protected:
  scoped_ptr<PhysicalSocketServer> pss_;
  scoped_ptr<VirtualSocketServer> vss_;
  AsyncSocket* socket_;
  scoped_ptr<AsyncUDPSocket> udp_socket_;
  bool ready_to_send_;
};

TEST_F(AsyncUdpSocketTest, OnWriteEvent) {
  EXPECT_FALSE(ready_to_send_);
  socket_->SignalWriteEvent(socket_);
  EXPECT_TRUE(ready_to_send_);
}

}  // namespace sm