// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#import <Foundation/Foundation.h>

#import "sm/base/scoped_autorelease_pool.h"

namespace sm {

ScopedAutoreleasePool::ScopedAutoreleasePool() {
  pool_ = [[NSAutoreleasePool alloc] init];
}

ScopedAutoreleasePool::~ScopedAutoreleasePool() {
  [pool_ drain];
}

}  // namespace sm
