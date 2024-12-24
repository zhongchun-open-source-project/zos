// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/http_request.h"

#include "sm/base/common.h"
#include "sm/base/firewall_socket_server.h"
#include "sm/base/http_client.h"
#include "sm/base/logging.h"
#include "sm/base/physical_socket_server.h"
#include "sm/base/socket_adapters.h"
#include "sm/base/socket_pool.h"
#include "sm/base/ssl_adapter.h"

using namespace sm;

///////////////////////////////////////////////////////////////////////////////
// HttpMonitor
///////////////////////////////////////////////////////////////////////////////

HttpMonitor::HttpMonitor(SocketServer *ss) {
  ASSERT(Thread::Current() != NULL);
  ss_ = ss;
  reset();
}

void HttpMonitor::Connect(HttpClient *http) {
  http->SignalHttpClientComplete.connect(this,
    &HttpMonitor::OnHttpClientComplete);
}

void HttpMonitor::OnHttpClientComplete(HttpClient * http, HttpErrorType error) {
  complete_ = true;
  error_ = error;
  ss_->WakeUp();
}

///////////////////////////////////////////////////////////////////////////////
// HttpRequest
///////////////////////////////////////////////////////////////////////////////

const int kDefaultHTTPTimeout = 30 * 1000; // 30 sec

HttpRequest::HttpRequest(const std::string& user_agent)
    : firewall_(0),
      port_(80),
      secure_(false),
      timeout_(kDefaultHTTPTimeout),
      client_(user_agent.c_str(), NULL),
      error_(HE_NONE) {}

HttpRequest::~HttpRequest() = default;

void HttpRequest::Send() {
  // TODO: Rewrite this to use the thread's native socket server, and a more
  // natural flow?

  PhysicalSocketServer physical;
  SocketServer * ss = &physical;
  if (firewall_) {
    ss = new FirewallSocketServer(ss, firewall_);
  }

  SslSocketFactory factory(ss, client_.agent());
  factory.SetProxy(proxy_);
  if (secure_)
    factory.UseSSL(host_.c_str());

  //factory.SetLogging("HttpRequest");

  ReuseSocketPool pool(&factory);
  client_.set_pool(&pool);

  bool transparent_proxy = (port_ == 80) && ((proxy_.type == PROXY_HTTPS) ||
                           (proxy_.type == PROXY_UNKNOWN));

  if (transparent_proxy) {
    client_.set_proxy(proxy_);
  }
  client_.set_redirect_action(HttpClient::REDIRECT_ALWAYS);

  SocketAddress server(host_, port_);
  client_.set_server(server);

  LOG(LS_INFO) << "HttpRequest start: " << host_ + client_.request().path;

  HttpMonitor monitor(ss);
  monitor.Connect(&client_);
  client_.start();
  ss->Wait(timeout_, true);
  if (!monitor.done()) {
    LOG(LS_INFO) << "HttpRequest request timed out";
    client_.reset();
    return;
  }

  set_error(monitor.error());
  if (error_) {
    LOG(LS_INFO) << "HttpRequest request error: " << error_;
    return;
  }

  std::string value;
  if (client_.response().hasHeader(HH_LOCATION, &value)) {
    response_redirect_ = value.c_str();
  }
}
