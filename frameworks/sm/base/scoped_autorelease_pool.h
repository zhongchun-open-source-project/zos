// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

// Automatically initialize and and free an autoreleasepool. Never allocate
// an instance of this class using "new" - that will result in a compile-time
// error. Only use it as a stack object.
//
// Note: NSAutoreleasePool docs say that you should not normally need to
// declare an NSAutoreleasePool as a member of an object - but there's nothing
// that indicates it will be a problem, as long as the stack lifetime of the
// pool exactly matches the stack lifetime of the object.

#ifndef SM_BASE_SCOPED_AUTORELEASE_POOL_H__
#define SM_BASE_SCOPED_AUTORELEASE_POOL_H__

#if defined(SM_MAC)

#include "sm/base/common.h"

// This header may be included from Obj-C files or C++ files.
#ifdef __OBJC__
@class NSAutoreleasePool;
#else
class NSAutoreleasePool;
#endif

namespace sm {

class ScopedAutoreleasePool {
 public:
  ScopedAutoreleasePool();
  ~ScopedAutoreleasePool();

 private:
  // Declaring private overrides of new and delete here enforces the "only use
  // as a stack object" discipline.
  //
  // Note: new is declared as "throw()" to get around a gcc warning about new
  // returning NULL, but this method will never get called and therefore will
  // never actually throw any exception.
  void* operator new(size_t size) throw() { return NULL; }
  void operator delete (void* ptr) {}

  NSAutoreleasePool* pool_;

  SM_DISALLOW_COPY_AND_ASSIGN(ScopedAutoreleasePool);
};

}  // namespace sm

#endif  // SM_MAC
#endif  // SM_BASE_SCOPED_AUTORELEASE_POOL_H__
