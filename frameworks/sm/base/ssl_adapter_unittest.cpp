// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include <string>

#include "sm/base/gunit.h"
#include "sm/base/ip_address.h"
#include "sm/base/socket_stream.h"
#include "sm/base/ssl_adapter.h"
#include "sm/base/ssl_stream_adapter.h"
#include "sm/base/ssl_identity.h"
#include "sm/base/stream.h"
#include "sm/base/virtual_socket_server.h"

static const int kTimeout = 5000;

static AsyncSocket* CreateSocket(const SSLMode& ssl_mode) {
  SocketAddress address(IPAddress(INADDR_ANY), 0);

  AsyncSocket* socket = Thread::Current()->
      socketserver()->CreateAsyncSocket(
      address.family(), (ssl_mode == SSL_MODE_DTLS) ?
      SOCK_DGRAM : SOCK_STREAM);
  socket->Bind(address);

  return socket;
}

static std::string GetSSLProtocolName(const SSLMode& ssl_mode) {
  return (ssl_mode == SSL_MODE_DTLS) ? "DTLS" : "TLS";
}

class SSLAdapterTestDummyClient : public sigslot::has_slots<> {
 public:
  explicit SSLAdapterTestDummyClient(const SSLMode& ssl_mode)
      : ssl_mode_(ssl_mode) {
    AsyncSocket* socket = CreateSocket(ssl_mode_);

    ssl_adapter_.reset(SSLAdapter::Create(socket));

    ssl_adapter_->SetMode(ssl_mode_);

    // Ignore any certificate errors for the purpose of testing.
    // Note: We do this only because we don't have a real certificate.
    // NEVER USE THIS IN PRODUCTION CODE!
    ssl_adapter_->set_ignore_bad_cert(true);

    ssl_adapter_->SignalReadEvent.connect(this,
        &SSLAdapterTestDummyClient::OnSSLAdapterReadEvent);
    ssl_adapter_->SignalCloseEvent.connect(this,
        &SSLAdapterTestDummyClient::OnSSLAdapterCloseEvent);
  }

  SocketAddress GetAddress() const {
    return ssl_adapter_->GetLocalAddress();
  }

  AsyncSocket::ConnState GetState() const {
    return ssl_adapter_->GetState();
  }

  const std::string& GetReceivedData() const {
    return data_;
  }

  int Connect(const std::string& hostname, const SocketAddress& address) {
    LOG(LS_INFO) << "Initiating connection with " << address;

    int rv = ssl_adapter_->Connect(address);

    if (rv == 0) {
      LOG(LS_INFO) << "Starting " << GetSSLProtocolName(ssl_mode_)
          << " handshake with " << hostname;

      if (ssl_adapter_->StartSSL(hostname.c_str(), false) != 0) {
        return -1;
      }
    }

    return rv;
  }

  int Close() {
    return ssl_adapter_->Close();
  }

  int Send(const std::string& message) {
    LOG(LS_INFO) << "Client sending '" << message << "'";

    return ssl_adapter_->Send(message.data(), message.length());
  }

  void OnSSLAdapterReadEvent(AsyncSocket* socket) {
    char buffer[4096] = "";

    // Read data received from the server and store it in our internal buffer.
    int read = socket->Recv(buffer, sizeof(buffer) - 1);
    if (read != -1) {
      buffer[read] = '\0';

      LOG(LS_INFO) << "Client received '" << buffer << "'";

      data_ += buffer;
    }
  }

  void OnSSLAdapterCloseEvent(AsyncSocket* socket, int error) {
    // OpenSSLAdapter signals handshake failure with a close event, but without
    // closing the socket! Let's close the socket here. This way GetState() can
    // return CS_CLOSED after failure.
    if (socket->GetState() != AsyncSocket::CS_CLOSED) {
      socket->Close();
    }
  }

 private:
  const SSLMode ssl_mode_;

  scoped_ptr<SSLAdapter> ssl_adapter_;

  std::string data_;
};

class SSLAdapterTestDummyServer : public sigslot::has_slots<> {
 public:
  explicit SSLAdapterTestDummyServer(const SSLMode& ssl_mode,
                                     const KeyParams& key_params)
      : ssl_mode_(ssl_mode) {
    // Generate a key pair and a certificate for this host.
    ssl_identity_.reset(SSLIdentity::Generate(GetHostname(), key_params));

    server_socket_.reset(CreateSocket(ssl_mode_));

    if (ssl_mode_ == SSL_MODE_TLS) {
      server_socket_->SignalReadEvent.connect(this,
          &SSLAdapterTestDummyServer::OnServerSocketReadEvent);

      server_socket_->Listen(1);
    }

    LOG(LS_INFO) << ((ssl_mode_ == SSL_MODE_DTLS) ? "UDP" : "TCP")
        << " server listening on " << server_socket_->GetLocalAddress();
  }

  SocketAddress GetAddress() const {
    return server_socket_->GetLocalAddress();
  }

  std::string GetHostname() const {
    // Since we don't have a real certificate anyway, the value here doesn't
    // really matter.
    return "example.com";
  }

  const std::string& GetReceivedData() const {
    return data_;
  }

  int Send(const std::string& message) {
    if (ssl_stream_adapter_ == NULL
        || ssl_stream_adapter_->GetState() != SS_OPEN) {
      // No connection yet.
      return -1;
    }

    LOG(LS_INFO) << "Server sending '" << message << "'";

    size_t written;
    int error;

    StreamResult r = ssl_stream_adapter_->Write(message.data(),
        message.length(), &written, &error);
    if (r == SR_SUCCESS) {
      return written;
    } else {
      return -1;
    }
  }

  void AcceptConnection(const SocketAddress& address) {
    // Only a single connection is supported.
    ASSERT_TRUE(ssl_stream_adapter_ == NULL);

    // This is only for DTLS.
    ASSERT_EQ(SSL_MODE_DTLS, ssl_mode_);

    // Transfer ownership of the socket to the SSLStreamAdapter object.
    AsyncSocket* socket = server_socket_.release();

    socket->Connect(address);

    DoHandshake(socket);
  }

  void OnServerSocketReadEvent(AsyncSocket* socket) {
    // Only a single connection is supported.
    ASSERT_TRUE(ssl_stream_adapter_ == NULL);

    DoHandshake(server_socket_->Accept(NULL));
  }

  void OnSSLStreamAdapterEvent(StreamInterface* stream, int sig, int err) {
    if (sig & SE_READ) {
      char buffer[4096] = "";

      size_t read;
      int error;

      // Read data received from the client and store it in our internal
      // buffer.
      StreamResult r = stream->Read(buffer,
          sizeof(buffer) - 1, &read, &error);
      if (r == SR_SUCCESS) {
        buffer[read] = '\0';

        LOG(LS_INFO) << "Server received '" << buffer << "'";

        data_ += buffer;
      }
    }
  }

 private:
  void DoHandshake(AsyncSocket* socket) {
    SocketStream* stream = new SocketStream(socket);

    ssl_stream_adapter_.reset(SSLStreamAdapter::Create(stream));

    ssl_stream_adapter_->SetMode(ssl_mode_);
    ssl_stream_adapter_->SetServerRole();

    // SSLStreamAdapter is normally used for peer-to-peer communication, but
    // here we're testing communication between a client and a server
    // (e.g. a SM-based application and an RFC 5766 TURN server), where
    // clients are not required to provide a certificate during handshake.
    // Accordingly, we must disable client authentication here.
    ssl_stream_adapter_->set_client_auth_enabled(false);

    ssl_stream_adapter_->SetIdentity(ssl_identity_->GetReference());

    // Set a bogus peer certificate digest.
    unsigned char digest[20];
    size_t digest_len = sizeof(digest);
    ssl_stream_adapter_->SetPeerCertificateDigest(DIGEST_SHA_1, digest,
        digest_len);

    ssl_stream_adapter_->StartSSLWithPeer();

    ssl_stream_adapter_->SignalEvent.connect(this,
        &SSLAdapterTestDummyServer::OnSSLStreamAdapterEvent);
  }

  const SSLMode ssl_mode_;

  scoped_ptr<AsyncSocket> server_socket_;
  scoped_ptr<SSLStreamAdapter> ssl_stream_adapter_;

  scoped_ptr<SSLIdentity> ssl_identity_;

  std::string data_;
};

class SSLAdapterTestBase : public testing::Test,
                           public sigslot::has_slots<> {
 public:
  explicit SSLAdapterTestBase(const SSLMode& ssl_mode,
                              const KeyParams& key_params)
      : ssl_mode_(ssl_mode),
        ss_scope_(new VirtualSocketServer(NULL)),
        server_(new SSLAdapterTestDummyServer(ssl_mode_, key_params)),
        client_(new SSLAdapterTestDummyClient(ssl_mode_)),
        handshake_wait_(kTimeout) {}

  void SetHandshakeWait(int wait) {
    handshake_wait_ = wait;
  }

  void TestHandshake(bool expect_success) {
    int rv;

    // The initial state is CS_CLOSED
    ASSERT_EQ(AsyncSocket::CS_CLOSED, client_->GetState());

    rv = client_->Connect(server_->GetHostname(), server_->GetAddress());
    ASSERT_EQ(0, rv);

    // Now the state should be CS_CONNECTING
    ASSERT_EQ(AsyncSocket::CS_CONNECTING, client_->GetState());

    if (ssl_mode_ == SSL_MODE_DTLS) {
      // For DTLS, call AcceptConnection() with the client's address.
      server_->AcceptConnection(client_->GetAddress());
    }

    if (expect_success) {
      // If expecting success, the client should end up in the CS_CONNECTED
      // state after handshake.
      EXPECT_EQ_WAIT(AsyncSocket::CS_CONNECTED, client_->GetState(),
          handshake_wait_);

      LOG(LS_INFO) << GetSSLProtocolName(ssl_mode_) << " handshake complete.";

    } else {
      // On handshake failure the client should end up in the CS_CLOSED state.
      EXPECT_EQ_WAIT(AsyncSocket::CS_CLOSED, client_->GetState(),
          handshake_wait_);

      LOG(LS_INFO) << GetSSLProtocolName(ssl_mode_) << " handshake failed.";
    }
  }

  void TestTransfer(const std::string& message) {
    int rv;

    rv = client_->Send(message);
    ASSERT_EQ(static_cast<int>(message.length()), rv);

    // The server should have received the client's message.
    EXPECT_EQ_WAIT(message, server_->GetReceivedData(), kTimeout);

    rv = server_->Send(message);
    ASSERT_EQ(static_cast<int>(message.length()), rv);

    // The client should have received the server's message.
    EXPECT_EQ_WAIT(message, client_->GetReceivedData(), kTimeout);

    LOG(LS_INFO) << "Transfer complete.";
  }

 private:
  const SSLMode ssl_mode_;

  const SocketServerScope ss_scope_;

  scoped_ptr<SSLAdapterTestDummyServer> server_;
  scoped_ptr<SSLAdapterTestDummyClient> client_;

  int handshake_wait_;
};

class SSLAdapterTestTLS_RSA : public SSLAdapterTestBase {
 public:
  SSLAdapterTestTLS_RSA()
      : SSLAdapterTestBase(SSL_MODE_TLS, KeyParams::RSA()) {}
};

class SSLAdapterTestTLS_ECDSA : public SSLAdapterTestBase {
 public:
  SSLAdapterTestTLS_ECDSA()
      : SSLAdapterTestBase(SSL_MODE_TLS, KeyParams::ECDSA()) {}
};

class SSLAdapterTestDTLS_RSA : public SSLAdapterTestBase {
 public:
  SSLAdapterTestDTLS_RSA()
      : SSLAdapterTestBase(SSL_MODE_DTLS, KeyParams::RSA()) {}
};

class SSLAdapterTestDTLS_ECDSA : public SSLAdapterTestBase {
 public:
  SSLAdapterTestDTLS_ECDSA()
      : SSLAdapterTestBase(SSL_MODE_DTLS, KeyParams::ECDSA()) {}
};

#if SSL_USE_OPENSSL

// Basic tests: TLS

// Test that handshake works, using RSA
TEST_F(SSLAdapterTestTLS_RSA, TestTLSConnect) {
  TestHandshake(true);
}

// Test that handshake works, using ECDSA
TEST_F(SSLAdapterTestTLS_ECDSA, TestTLSConnect) {
  TestHandshake(true);
}

// Test transfer between client and server, using RSA
TEST_F(SSLAdapterTestTLS_RSA, TestTLSTransfer) {
  TestHandshake(true);
  TestTransfer("Hello, world!");
}

// Test transfer between client and server, using ECDSA
TEST_F(SSLAdapterTestTLS_ECDSA, TestTLSTransfer) {
  TestHandshake(true);
  TestTransfer("Hello, world!");
}

// Basic tests: DTLS

// Test that handshake works, using RSA
TEST_F(SSLAdapterTestDTLS_RSA, TestDTLSConnect) {
  TestHandshake(true);
}

// Test that handshake works, using ECDSA
TEST_F(SSLAdapterTestDTLS_ECDSA, TestDTLSConnect) {
  TestHandshake(true);
}

// Test transfer between client and server, using RSA
TEST_F(SSLAdapterTestDTLS_RSA, TestDTLSTransfer) {
  TestHandshake(true);
  TestTransfer("Hello, world!");
}

// Test transfer between client and server, using ECDSA
TEST_F(SSLAdapterTestDTLS_ECDSA, TestDTLSTransfer) {
  TestHandshake(true);
  TestTransfer("Hello, world!");
}

#endif  // SSL_USE_OPENSSL
