// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_SOCKET_ADDRESS_PAIR_H_
#define SM_BASE_SOCKET_ADDRESS_PAIR_H_

#include "sm/base/socket_address.h"

namespace sm {

// Records a pair (source,destination) of socket addresses.  The two addresses
// identify a connection between two machines.  (For UDP, this "connection" is
// not maintained explicitly in a socket.)
class SocketAddressPair {
public:
  SocketAddressPair() {}
  SocketAddressPair(const SocketAddress& srs, const SocketAddress& dest);

  const SocketAddress& source() const { return src_; }
  const SocketAddress& destination() const { return dest_; }

  bool operator ==(const SocketAddressPair& r) const;
  bool operator <(const SocketAddressPair& r) const;

  size_t Hash() const;

private:
  SocketAddress src_;
  SocketAddress dest_;
};

} // namespace sm

#endif // SM_BASE_SOCKET_ADDRESS_PAIR_H_
