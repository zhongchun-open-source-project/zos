// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef _PROXY_DETECT_H_
#define _PROXY_DETECT_H_

#include "sm/base/proxyinfo.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace sm {
// Auto-detect the proxy server.  Returns true if a proxy is configured,
// although hostname may be empty if the proxy is not required for
// the given URL.

bool GetProxySettingsForUrl(const char* agent, const char* url,
                            ProxyInfo* proxy,
                            bool long_operation = false);

}  // namespace sm

#endif  // _PROXY_DETECT_H_
