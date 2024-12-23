// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_HTTP_REQUEST_H_
#define SM_BASE_HTTP_REQUEST_H_

#include "sm/base/http_client.h"
#include "sm/base/logging.h"
#include "sm/base/proxy_info.h"
#include "sm/base/socket_server.h"
#include "sm/base/thread.h"
#include "sm/base/ssl_socket_factory.h"  // Deprecated include

namespace sm {

///////////////////////////////////////////////////////////////////////////////
// HttpRequest
///////////////////////////////////////////////////////////////////////////////

class FirewallManager;
class MemoryStream;

class HttpRequest {
public:
  HttpRequest(const std::string &user_agent);
  ~HttpRequest();

  void Send();

  void set_proxy(const ProxyInfo& proxy) {
    proxy_ = proxy;
  }
  void set_firewall(FirewallManager * firewall) {
    firewall_ = firewall;
  }

  // The DNS name of the host to connect to.
  const std::string& host() { return host_; }
  void set_host(const std::string& host) { host_ = host; }

  // The port to connect to on the target host.
  int port() { return port_; }
  void set_port(int port) { port_ = port; }

   // Whether the request should use SSL.
  bool secure() { return secure_; }
  void set_secure(bool secure) { secure_ = secure; }

  // Returns the redirect when redirection occurs
  const std::string& response_redirect() { return response_redirect_; }

  // Time to wait on the download, in ms.  Default is 5000 (5s)
  int timeout() { return timeout_; }
  void set_timeout(int timeout) { timeout_ = timeout; }

  // Fail redirects to allow analysis of redirect urls, etc.
  bool fail_redirect() const { return fail_redirect_; }
  void set_fail_redirect(bool fail_redirect) { fail_redirect_ = fail_redirect; }

  HttpRequestData& request() { return client_.request(); }
  HttpResponseData& response() { return client_.response(); }
  HttpErrorType error() { return error_; }

protected:
  void set_error(HttpErrorType error) { error_ = error; }

private:
  ProxyInfo proxy_;
  FirewallManager * firewall_;
  std::string host_;
  int port_;
  bool secure_;
  int timeout_;
  bool fail_redirect_;
  HttpClient client_;
  HttpErrorType error_;
  std::string response_redirect_;
};

///////////////////////////////////////////////////////////////////////////////
// HttpMonitor
///////////////////////////////////////////////////////////////////////////////

class HttpMonitor : public sigslot::has_slots<> {
public:
  HttpMonitor(SocketServer *ss);

  void reset() {
    complete_ = false;
    error_ = HE_DEFAULT;
  }

  bool done() const { return complete_; }
  HttpErrorType error() const { return error_; }

  void Connect(HttpClient* http);
  void OnHttpClientComplete(HttpClient * http, HttpErrorType error);

private:
  bool complete_;
  HttpErrorType error_;
  SocketServer *ss_;
};

///////////////////////////////////////////////////////////////////////////////

}  // namespace sm

#endif  // SM_BASE_HTTP_REQUEST_H_
