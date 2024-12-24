// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/ifaddrs_converter.h"

namespace sm {

IfAddrsConverter::IfAddrsConverter() {}

IfAddrsConverter::~IfAddrsConverter() {}

bool IfAddrsConverter::ConvertIfAddrsToIPAddress(
    const struct ifaddrs* interface,
    InterfaceAddress* ip,
    IPAddress* mask) {
  switch (interface->ifa_addr->sa_family) {
    case AF_INET: {
      *ip = IPAddress(
          reinterpret_cast<sockaddr_in*>(interface->ifa_addr)->sin_addr);
      *mask = IPAddress(
          reinterpret_cast<sockaddr_in*>(interface->ifa_netmask)->sin_addr);
      return true;
    }
    case AF_INET6: {
      int ip_attributes = IPV6_ADDRESS_FLAG_NONE;
      if (!ConvertNativeAttributesToIPAttributes(interface, &ip_attributes)) {
        return false;
      }
      *ip = InterfaceAddress(
          reinterpret_cast<sockaddr_in6*>(interface->ifa_addr)->sin6_addr,
          ip_attributes);
      *mask = IPAddress(
          reinterpret_cast<sockaddr_in6*>(interface->ifa_netmask)->sin6_addr);
      return true;
    }
    default: { return false; }
  }
}

bool IfAddrsConverter::ConvertNativeAttributesToIPAttributes(
    const struct ifaddrs* interface,
    int* ip_attributes) {
  *ip_attributes = IPV6_ADDRESS_FLAG_NONE;
  return true;
}

#if !defined(SM_MAC)
// For MAC and IOS, it's defined in macifaddrs_converter.cc
IfAddrsConverter* CreateIfAddrsConverter() {
  return new IfAddrsConverter();
}
#endif
}  // namespace sm
