// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

// Helper function for using Cocoa with Posix threading.

#import <assert.h>
#import <Foundation/Foundation.h>

#import "sm/base/mac_cocoa_thread_helper.h"

namespace sm {

// Cocoa must be "put into multithreading mode" before Cocoa functionality can
// be used on POSIX threads. The way to do that is to spawn one thread that may
// immediately exit.
void InitCocoaMultiThreading() {
  if ([NSThread isMultiThreaded] == NO) {
    // The sole purpose of this autorelease pool is to avoid a console
    // message on Leopard that tells us we're autoreleasing the thread
    // with no autorelease pool in place.
    // Doing NSAutoreleasePool* hack = [[NSAutoreleasePool alloc] init];
    // causes unused variable error.
    NSAutoreleasePool* hack;
    hack = [[NSAutoreleasePool alloc] init];
    [NSThread detachNewThreadSelector:@selector(class)
                             toTarget:[NSObject class]
                           withObject:nil];
    [hack drain];
  }

  assert([NSThread isMultiThreaded]);
}

}  // namespace sm
