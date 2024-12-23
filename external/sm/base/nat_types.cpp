// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include <assert.h>

#include "sm/base/nat_types.h"

namespace sm {

class SymmetricNAT : public NAT {
public:
 bool IsSymmetric() override { return true; }
 bool FiltersIP() override { return true; }
 bool FiltersPort() override { return true; }
};

class OpenConeNAT : public NAT {
public:
 bool IsSymmetric() override { return false; }
 bool FiltersIP() override { return false; }
 bool FiltersPort() override { return false; }
};

class AddressRestrictedNAT : public NAT {
public:
 bool IsSymmetric() override { return false; }
 bool FiltersIP() override { return true; }
 bool FiltersPort() override { return false; }
};

class PortRestrictedNAT : public NAT {
public:
 bool IsSymmetric() override { return false; }
 bool FiltersIP() override { return true; }
 bool FiltersPort() override { return true; }
};

NAT* NAT::Create(NATType type) {
  switch (type) {
  case NAT_OPEN_CONE:       return new OpenConeNAT();
  case NAT_ADDR_RESTRICTED: return new AddressRestrictedNAT();
  case NAT_PORT_RESTRICTED: return new PortRestrictedNAT();
  case NAT_SYMMETRIC:       return new SymmetricNAT();
  default: assert(0);       return 0;
  }
}

} // namespace sm
