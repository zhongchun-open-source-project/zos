// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/gunit.h"
#include "sm/base/net_helpers.h"
#include "sm/base/physical_socket_server.h"
#include "sm/base/test_client.h"
#include "sm/base/test_echo_server.h"
#include "sm/base/thread.h"

using namespace sm;

void TestUdpInternal(const SocketAddress& loopback) {
  Thread *main = Thread::Current();
  AsyncSocket* socket = main->socketserver()
      ->CreateAsyncSocket(loopback.family(), SOCK_DGRAM);
  socket->Bind(loopback);

  TestClient client(new AsyncUDPSocket(socket));
  SocketAddress addr = client.address(), from;
  EXPECT_EQ(3, client.SendTo("foo", 3, addr));
  EXPECT_TRUE(client.CheckNextPacket("foo", 3, &from));
  EXPECT_EQ(from, addr);
  EXPECT_TRUE(client.CheckNoPacket());
}

void TestTcpInternal(const SocketAddress& loopback) {
  Thread *main = Thread::Current();
  TestEchoServer server(main, loopback);

  AsyncSocket* socket = main->socketserver()
      ->CreateAsyncSocket(loopback.family(), SOCK_STREAM);
  AsyncTCPSocket* tcp_socket = AsyncTCPSocket::Create(
      socket, loopback, server.address());
  ASSERT_TRUE(tcp_socket != NULL);

  TestClient client(tcp_socket);
  SocketAddress addr = client.address(), from;
  EXPECT_TRUE(client.CheckConnected());
  EXPECT_EQ(3, client.Send("foo", 3));
  EXPECT_TRUE(client.CheckNextPacket("foo", 3, &from));
  EXPECT_EQ(from, server.address());
  EXPECT_TRUE(client.CheckNoPacket());
}

// Tests whether the TestClient can send UDP to itself.
TEST(TestClientTest, TestUdpIPv4) {
  TestUdpInternal(SocketAddress("127.0.0.1", 0));
}

#if defined(SM_LINUX)
#define MAYBE_TestUdpIPv6 DISABLED_TestUdpIPv6
#else
#define MAYBE_TestUdpIPv6 TestUdpIPv6
#endif
TEST(TestClientTest, MAYBE_TestUdpIPv6) {
  if (HasIPv6Enabled()) {
    TestUdpInternal(SocketAddress("::1", 0));
  } else {
    LOG(LS_INFO) << "Skipping IPv6 test.";
  }
}

// Tests whether the TestClient can connect to a server and exchange data.
TEST(TestClientTest, TestTcpIPv4) {
  TestTcpInternal(SocketAddress("127.0.0.1", 0));
}

#if defined(SM_LINUX)
#define MAYBE_TestTcpIPv6 DISABLED_TestTcpIPv6
#else
#define MAYBE_TestTcpIPv6 TestTcpIPv6
#endif
TEST(TestClientTest, MAYBE_TestTcpIPv6) {
  if (HasIPv6Enabled()) {
    TestTcpInternal(SocketAddress("::1", 0));
  } else {
    LOG(LS_INFO) << "Skipping IPv6 test.";
  }
}
