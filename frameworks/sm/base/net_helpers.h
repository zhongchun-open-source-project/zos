// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_NET_HELPERS_H_
#define SM_BASE_NET_HELPERS_H_

#if defined(SM_POSIX)
#include <netdb.h>
#include <stddef.h>
#elif SM_WIN
#include <winsock2.h>  // NOLINT
#endif

#include <list>

#include "sm/base/async_resolver_interface.h"
#include "sm/base/signal_thread.h"
#include "sm/base/sigslot.h"
#include "sm/base/socket_address.h"

namespace sm {

class AsyncResolverTest;

// AsyncResolver will perform async DNS resolution, signaling the result on
// the SignalDone from AsyncResolverInterface when the operation completes.
class AsyncResolver : public SignalThread, public AsyncResolverInterface {
 public:
  AsyncResolver();
  ~AsyncResolver() override;

  void Start(const SocketAddress& addr) override;
  bool GetResolvedAddress(int family, SocketAddress* addr) const override;
  int GetError() const override;
  void Destroy(bool wait) override;

  const std::vector<IPAddress>& addresses() const { return addresses_; }
  void set_error(int error) { error_ = error; }

 protected:
  void DoWork() override;
  void OnWorkDone() override;

 private:
  SocketAddress addr_;
  std::vector<IPAddress> addresses_;
  int error_;
};

// sm namespaced wrappers for inet_ntop and inet_pton so we can avoid
// the windows-native versions of these.
const char* inet_ntop(int af, const void *src, char* dst, socklen_t size);
int inet_pton(int af, const char* src, void *dst);

bool HasIPv6Enabled();
}  // namespace sm

#endif  // SM_BASE_NET_HELPERS_H_
