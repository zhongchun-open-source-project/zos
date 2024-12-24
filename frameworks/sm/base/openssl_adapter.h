// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_OPENSSL_ADAPTER_H_
#define SM_BASE_OPENSSL_ADAPTER_H_

#include <string>
#include "sm/base/message_handler.h"
#include "sm/base/message_queue.h"
#include "sm/base/ssl_adapter.h"

typedef struct ssl_st SSL;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct x509_store_ctx_st X509_STORE_CTX;

namespace sm {

///////////////////////////////////////////////////////////////////////////////

class OpenSSLAdapter : public SSLAdapter, public MessageHandler {
public:
  static bool InitializeSSL(VerificationCallback callback);
  static bool InitializeSSLThread();
  static bool CleanupSSL();

  OpenSSLAdapter(AsyncSocket* socket);
  ~OpenSSLAdapter() override;

  void SetMode(SSLMode mode) override;
  int StartSSL(const char* hostname, bool restartable) override;
  int Send(const void* pv, size_t cb) override;
  int SendTo(const void* pv, size_t cb, const SocketAddress& addr) override;
  int Recv(void* pv, size_t cb) override;
  int RecvFrom(void* pv, size_t cb, SocketAddress* paddr) override;
  int Close() override;

  // Note that the socket returns ST_CONNECTING while SSL is being negotiated.
  ConnState GetState() const override;

protected:
 void OnConnectEvent(AsyncSocket* socket) override;
 void OnReadEvent(AsyncSocket* socket) override;
 void OnWriteEvent(AsyncSocket* socket) override;
 void OnCloseEvent(AsyncSocket* socket, int err) override;

private:
  enum SSLState {
    SSL_NONE, SSL_WAIT, SSL_CONNECTING, SSL_CONNECTED, SSL_ERROR
  };

  enum { MSG_TIMEOUT };

  int BeginSSL();
  int ContinueSSL();
  void Error(const char* context, int err, bool signal = true);
  void Cleanup();

  void OnMessage(Message* msg) override;

  static bool VerifyServerName(SSL* ssl, const char* host,
                               bool ignore_bad_cert);
  bool SSLPostConnectionCheck(SSL* ssl, const char* host);
#if !defined(NDEBUG)
  static void SSLInfoCallback(const SSL* s, int where, int ret);
#endif
  static int SSLVerifyCallback(int ok, X509_STORE_CTX* store);
  static VerificationCallback custom_verify_callback_;
  friend class OpenSSLStreamAdapter;  // for custom_verify_callback_;

  static bool ConfigureTrustedRootCertificates(SSL_CTX* ctx);
  SSL_CTX* SetupSSLContext();

  SSLState state_;
  bool ssl_read_needs_write_;
  bool ssl_write_needs_read_;
  // If true, socket will retain SSL configuration after Close.
  bool restartable_;

  SSL* ssl_;
  SSL_CTX* ssl_ctx_;
  std::string ssl_host_name_;
  // Do DTLS or not
  SSLMode ssl_mode_;

  bool custom_verification_succeeded_;
};

/////////////////////////////////////////////////////////////////////////////

} // namespace sm

#endif // SM_BASE_OPENSSL_ADAPTER_H_
