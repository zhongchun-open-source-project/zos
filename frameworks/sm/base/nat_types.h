// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_NAT_TYPE_H_
#define SM_BASE_NAT_TYPE_H_

namespace sm {

/* Identifies each type of NAT that can be simulated. */
enum NATType {
  NAT_OPEN_CONE,
  NAT_ADDR_RESTRICTED,
  NAT_PORT_RESTRICTED,
  NAT_SYMMETRIC
};

// Implements the rules for each specific type of NAT.
class NAT {
public:
  virtual ~NAT() { }

  // Determines whether this NAT uses both source and destination address when
  // checking whether a mapping already exists.
  virtual bool IsSymmetric() = 0;

  // Determines whether this NAT drops packets received from a different IP
  // the one last sent to.
  virtual bool FiltersIP() = 0;

  // Determines whether this NAT drops packets received from a different port
  // the one last sent to.
  virtual bool FiltersPort() = 0;

  // Returns an implementation of the given type of NAT.
  static NAT* Create(NATType type);
};

} // namespace sm

#endif // SM_BASE_NAT_TYPE_H_
