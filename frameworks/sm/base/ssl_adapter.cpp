// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#if HAVE_CONFIG_H
#include "config.h"
#endif  // HAVE_CONFIG_H

#include "sm/base/ssl_adapter.h"

#include "sm/base/ssl_config.h"

#if SSL_USE_OPENSSL

#include "openssl_adapter.h"

#endif

///////////////////////////////////////////////////////////////////////////////

namespace sm {

SSLAdapter*
SSLAdapter::Create(AsyncSocket* socket) {
#if SSL_USE_OPENSSL
  return new OpenSSLAdapter(socket);
#else  // !SSL_USE_OPENSSL
  delete socket;
  return NULL;
#endif  // SSL_USE_OPENSSL
}

///////////////////////////////////////////////////////////////////////////////

#if SSL_USE_OPENSSL

bool InitializeSSL(VerificationCallback callback) {
  return OpenSSLAdapter::InitializeSSL(callback);
}

bool InitializeSSLThread() {
  return OpenSSLAdapter::InitializeSSLThread();
}

bool CleanupSSL() {
  return OpenSSLAdapter::CleanupSSL();
}

#else  // !SSL_USE_OPENSSL

bool InitializeSSL(VerificationCallback callback) {
  return true;
}

bool InitializeSSLThread() {
  return true;
}

bool CleanupSSL() {
  return true;
}

#endif  // SSL_USE_OPENSSL

///////////////////////////////////////////////////////////////////////////////

}  // namespace sm
