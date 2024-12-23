// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_ASYNC_RESOLVER_INTERFACE_H_
#define SM_BASE_ASYNC_RESOLVER_INTERFACE_H_

#include "sm/base/sigslot.h"
#include "sm/base/socket_address.h"

namespace sm {

// This interface defines the methods to resolve the address asynchronously.
class AsyncResolverInterface {
 public:
  AsyncResolverInterface();
  virtual ~AsyncResolverInterface();

  // Start address resolve process.
  virtual void Start(const SocketAddress& addr) = 0;
  // Returns top most resolved address of |family|
  virtual bool GetResolvedAddress(int family, SocketAddress* addr) const = 0;
  // Returns error from resolver.
  virtual int GetError() const = 0;
  // Delete the resolver.
  virtual void Destroy(bool wait) = 0;
  // Returns top most resolved IPv4 address if address is resolved successfully.
  // Otherwise returns address set in SetAddress.
  SocketAddress address() const {
    SocketAddress addr;
    GetResolvedAddress(AF_INET, &addr);
    return addr;
  }

  // This signal is fired when address resolve process is completed.
  sigslot::signal1<AsyncResolverInterface*> SignalDone;
};

}  // namespace sm

#endif // SM_BASE_ASYNC_RESOLVER_INTERFACE_H_
