// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_PROXY_INFO_H_
#define SM_BASE_PROXY_INFO_H_

#include <string>
#include "sm/base/socket_address.h"
#include "sm/base/crypt_string.h"

namespace sm {

enum ProxyType {
  PROXY_NONE,
  PROXY_HTTPS,
  PROXY_SOCKS5,
  PROXY_UNKNOWN
};
const char * ProxyToString(ProxyType proxy);

struct ProxyInfo {
  ProxyType type;
  SocketAddress address;
  std::string autoconfig_url;
  bool autodetect;
  std::string bypass_list;
  std::string username;
  CryptString password;

  ProxyInfo();
  ~ProxyInfo();
};

} // namespace sm

#endif // SM_BASE_PROXY_INFO_H_
