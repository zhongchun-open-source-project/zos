// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_AUTO_DETECT_PROXY_H_
#define SM_BASE_AUTO_DETECT_PROXY_H_

#include <string>

#include "sm/base/constructor_magic.h"
#include "sm/base/crypt_string.h"
#include "sm/base/proxy_detect.h"
#include "sm/base/proxy_info.h"
#include "sm/base/signal_thread.h"

namespace sm {

///////////////////////////////////////////////////////////////////////////////
// AutoDetectProxy
///////////////////////////////////////////////////////////////////////////////

class AsyncResolverInterface;
class AsyncSocket;

class AutoDetectProxy : public SignalThread {
 public:
  explicit AutoDetectProxy(const std::string& user_agent);

  const ProxyInfo& proxy() const { return proxy_; }

  void set_server_url(const std::string& url) {
    server_url_ = url;
  }
  void set_proxy(const SocketAddress& proxy) {
    proxy_.type = PROXY_UNKNOWN;
    proxy_.address = proxy;
  }
  void set_auth_info(bool use_auth, const std::string& username,
                     const CryptString& password) {
    if (use_auth) {
      proxy_.username = username;
      proxy_.password = password;
    }
  }
  // Default implementation of GetProxySettingsForUrl. Override for special
  // implementation.
  virtual bool GetProxyForUrl(const char* agent,
                              const char* url,
                              ProxyInfo* proxy);
  enum { MSG_TIMEOUT = SignalThread::ST_MSG_FIRST_AVAILABLE,
         MSG_UNRESOLVABLE,
         ADP_MSG_FIRST_AVAILABLE};

 protected:
  ~AutoDetectProxy() override;

  // SignalThread Interface
  void DoWork() override;
  void OnMessage(Message* msg) override;

  void Next();
  void Complete(ProxyType type);

  void OnConnectEvent(AsyncSocket * socket);
  void OnReadEvent(AsyncSocket * socket);
  void OnCloseEvent(AsyncSocket * socket, int error);
  void OnResolveResult(AsyncResolverInterface* resolver);
  bool DoConnect();

 private:
  std::string agent_;
  std::string server_url_;
  ProxyInfo proxy_;
  AsyncResolverInterface* resolver_;
  AsyncSocket* socket_;
  int next_;

  SM_DISALLOW_IMPLICIT_CONSTRUCTORS(AutoDetectProxy);
};

}  // namespace sm

#endif  // SM_BASE_AUTO_DETECT_PROXY_H_
