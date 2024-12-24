// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include <string>
#include "sm/base/auto_detect_proxy.h"
#include "sm/base/gunit.h"
#include "sm/base/http_server.h"
#include "sm/base/proxy_server.h"
#include "sm/base/socket_adapters.h"
#include "sm/base/test_client.h"
#include "sm/base/test_echo_server.h"
#include "sm/base/virtual_socket_server.h"

using Socket;
using Thread;
using SocketAddress;

static const SocketAddress kSocksProxyIntAddr("1.2.3.4", 1080);
static const SocketAddress kSocksProxyExtAddr("1.2.3.5", 0);
static const SocketAddress kHttpsProxyIntAddr("1.2.3.4", 443);
static const SocketAddress kHttpsProxyExtAddr("1.2.3.5", 0);
static const SocketAddress kBogusProxyIntAddr("1.2.3.4", 999);

// Used to run a proxy detect on the current thread. Otherwise we would need
// to make both threads share the same VirtualSocketServer.
class AutoDetectProxyRunner : public AutoDetectProxy {
 public:
  explicit AutoDetectProxyRunner(const std::string& agent)
      : AutoDetectProxy(agent) {}
  void Run() {
    DoWork();
    Thread::Current()->Restart();  // needed to reset the messagequeue
  }
};

// Sets up a virtual socket server and HTTPS/SOCKS5 proxy servers.
class ProxyTest : public testing::Test {
 public:
  ProxyTest() : ss_(new VirtualSocketServer(NULL)) {
    Thread::Current()->set_socketserver(ss_.get());
    socks_.reset(new SocksProxyServer(
        ss_.get(), kSocksProxyIntAddr, ss_.get(), kSocksProxyExtAddr));
    https_.reset(new HttpListenServer());
    https_->Listen(kHttpsProxyIntAddr);
  }
  ~ProxyTest() {
    Thread::Current()->set_socketserver(NULL);
  }

  SocketServer* ss() { return ss_.get(); }

  ProxyType DetectProxyType(const SocketAddress& address) {
    ProxyType type;
    AutoDetectProxyRunner* detect = new AutoDetectProxyRunner("unittest/1.0");
    detect->set_proxy(address);
    detect->Run();  // blocks until done
    type = detect->proxy().type;
    detect->Destroy(false);
    return type;
  }

 private:
  scoped_ptr<SocketServer> ss_;
  scoped_ptr<SocksProxyServer> socks_;
  // TODO: Make this a real HTTPS proxy server.
  scoped_ptr<HttpListenServer> https_;
};

// Tests whether we can use a SOCKS5 proxy to connect to a server.
TEST_F(ProxyTest, TestSocks5Connect) {
  AsyncSocket* socket =
      ss()->CreateAsyncSocket(kSocksProxyIntAddr.family(), SOCK_STREAM);
  AsyncSocksProxySocket* proxy_socket =
      new AsyncSocksProxySocket(socket, kSocksProxyIntAddr,
                                           "", CryptString());
  // TODO: IPv6-ize these tests when proxy supports IPv6.

  TestEchoServer server(Thread::Current(),
                                   SocketAddress(INADDR_ANY, 0));

  AsyncTCPSocket* packet_socket = AsyncTCPSocket::Create(
      proxy_socket, SocketAddress(INADDR_ANY, 0), server.address());
  EXPECT_TRUE(packet_socket != NULL);
  TestClient client(packet_socket);

  EXPECT_EQ(Socket::CS_CONNECTING, proxy_socket->GetState());
  EXPECT_TRUE(client.CheckConnected());
  EXPECT_EQ(Socket::CS_CONNECTED, proxy_socket->GetState());
  EXPECT_EQ(server.address(), client.remote_address());
  client.Send("foo", 3);
  EXPECT_TRUE(client.CheckNextPacket("foo", 3, NULL));
  EXPECT_TRUE(client.CheckNoPacket());
}

/*
// Tests whether we can use a HTTPS proxy to connect to a server.
TEST_F(ProxyTest, TestHttpsConnect) {
  AsyncSocket* socket = ss()->CreateAsyncSocket(SOCK_STREAM);
  AsyncHttpsProxySocket* proxy_socket = new AsyncHttpsProxySocket(
      socket, "unittest/1.0", kHttpsProxyIntAddress, "", CryptString());
  TestClient client(new AsyncTCPSocket(proxy_socket));
  TestEchoServer server(Thread::Current(), SocketAddress());

  EXPECT_TRUE(client.Connect(server.address()));
  EXPECT_TRUE(client.CheckConnected());
  EXPECT_EQ(server.address(), client.remote_address());
  client.Send("foo", 3);
  EXPECT_TRUE(client.CheckNextPacket("foo", 3, NULL));
  EXPECT_TRUE(client.CheckNoPacket());
}
*/

// Tests whether we can autodetect a SOCKS5 proxy.
TEST_F(ProxyTest, TestAutoDetectSocks5) {
  EXPECT_EQ(PROXY_SOCKS5, DetectProxyType(kSocksProxyIntAddr));
}

/*
// Tests whether we can autodetect a HTTPS proxy.
TEST_F(ProxyTest, TestAutoDetectHttps) {
  EXPECT_EQ(PROXY_HTTPS, DetectProxyType(kHttpsProxyIntAddr));
}
*/

// Tests whether we fail properly for no proxy.
TEST_F(ProxyTest, TestAutoDetectBogus) {
  EXPECT_EQ(PROXY_UNKNOWN, DetectProxyType(kBogusProxyIntAddr));
}
