// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_SSL_ADAPTER_H_
#define SM_BASE_SSL_ADAPTER_H_

#include "sm/base/async_socket.h"
#include "sm/base/ssl_stream_adapter.h"

namespace sm {

///////////////////////////////////////////////////////////////////////////////

class SSLAdapter : public AsyncSocketAdapter {
 public:
  explicit SSLAdapter(AsyncSocket* socket)
    : AsyncSocketAdapter(socket), ignore_bad_cert_(false) { }

  bool ignore_bad_cert() const { return ignore_bad_cert_; }
  void set_ignore_bad_cert(bool ignore) { ignore_bad_cert_ = ignore; }

  // Do DTLS or TLS (default is TLS, if unspecified)
  virtual void SetMode(SSLMode mode) = 0;

  // StartSSL returns 0 if successful.
  // If StartSSL is called while the socket is closed or connecting, the SSL
  // negotiation will begin as soon as the socket connects.
  virtual int StartSSL(const char* hostname, bool restartable) = 0;

  // Create the default SSL adapter for this platform. On failure, returns NULL
  // and deletes |socket|. Otherwise, the returned SSLAdapter takes ownership
  // of |socket|.
  static SSLAdapter* Create(AsyncSocket* socket);

 private:
  // If true, the server certificate need not match the configured hostname.
  bool ignore_bad_cert_;
};

///////////////////////////////////////////////////////////////////////////////

typedef bool (*VerificationCallback)(void* cert);

// Call this on the main thread, before using SSL.
// Call CleanupSSLThread when finished with SSL.
bool InitializeSSL(VerificationCallback callback = NULL);

// Call to initialize additional threads.
bool InitializeSSLThread();

// Call to cleanup additional threads, and also the main thread.
bool CleanupSSL();

///////////////////////////////////////////////////////////////////////////////

}  // namespace sm

#endif  // SM_BASE_SSL_ADAPTER_H_
