// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_REF_COUNT_H_
#define SM_BASE_REF_COUNT_H_

#include "sm/base/atomic32.h"

namespace sm {

// This class can be used for instantiating
// reference counted objects.
// int32_t AddRef() and int32_t Release().
// Usage:
// RefCountImpl<T>* implementation = new RefCountImpl<T>(p);
//
// Example:
// class MyInterface {
//  public:
//   virtual void DoSomething() = 0;
//   virtual int32_t AddRef() = 0;
//   virtual int32_t Release() = 0:
//  private:
//   virtual ~MyInterface(){};
// }
// class MyImplementation : public MyInterface {
//  public:
//   virtual DoSomething() { printf("hello"); };
// };
// MyImplementation* CreateMyImplementation() {
//   RefCountImpl<MyImplementation>* implementation =
//       new RefCountImpl<MyImplementation>();
//   return implementation;
// }

template <class T>
class RefCountImpl : public T {
 public:
  RefCountImpl() : ref_count_(0) {}

  template<typename P>
  explicit RefCountImpl(P p) : T(p), ref_count_(0) {}

  template<typename P1, typename P2>
  RefCountImpl(P1 p1, P2 p2) : T(p1, p2), ref_count_(0) {}

  template<typename P1, typename P2, typename P3>
  RefCountImpl(P1 p1, P2 p2, P3 p3) : T(p1, p2, p3), ref_count_(0) {}

  template<typename P1, typename P2, typename P3, typename P4>
  RefCountImpl(P1 p1, P2 p2, P3 p3, P4 p4) : T(p1, p2, p3, p4), ref_count_(0) {}

  template<typename P1, typename P2, typename P3, typename P4, typename P5>
  RefCountImpl(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
      : T(p1, p2, p3, p4, p5), ref_count_(0) {}

  int32_t AddRef() const override {
    return ++ref_count_;
  }

  int32_t Release() const override {
    int32_t ref_count;
    ref_count = --ref_count_;
    if (ref_count == 0)
      delete this;
    return ref_count;
  }

 protected:
  mutable Atomic32 ref_count_;
};

}  // namespace sm

#endif  // SM_BASE_REF_COUNT_H_
