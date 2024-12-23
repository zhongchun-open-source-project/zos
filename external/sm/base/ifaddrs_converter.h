// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_IFADDRS_CONVERTER_H_
#define SM_BASE_IFADDRS_CONVERTER_H_

#if defined(SM_ANDROID)
#include "sm/base/ifaddrs_android.h"
#else
#include <ifaddrs.h>
#endif  // SM_ANDROID

#include "sm/base/ip_address.h"

namespace sm {

// This class converts native interface addresses to our internal IPAddress
// class. Subclasses should override ConvertNativeToIPAttributes to implement
// the different ways of retrieving IPv6 attributes for various POSIX platforms.
class IfAddrsConverter {
 public:
  IfAddrsConverter();
  virtual ~IfAddrsConverter();
  virtual bool ConvertIfAddrsToIPAddress(const struct ifaddrs* interface,
                                         InterfaceAddress* ipaddress,
                                         IPAddress* mask);

 protected:
  virtual bool ConvertNativeAttributesToIPAttributes(
      const struct ifaddrs* interface,
      int* ip_attributes);
};

IfAddrsConverter* CreateIfAddrsConverter();

}  // namespace sm

#endif  // SM_BASE_IFADDRS_CONVERTER_H_
