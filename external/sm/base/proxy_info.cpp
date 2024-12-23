// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/proxy_info.h"

namespace sm {

const char * ProxyToString(ProxyType proxy) {
  const char * const PROXY_NAMES[] = { "none", "https", "socks5", "unknown" };
  return PROXY_NAMES[proxy];
}

ProxyInfo::ProxyInfo() : type(PROXY_NONE), autodetect(false) {
}
ProxyInfo::~ProxyInfo() = default;

} // namespace sm
