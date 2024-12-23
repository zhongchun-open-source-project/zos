// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#import "sm/base/mac_cocoa_socket_server.h"

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#include <assert.h>

#include "sm/base/scoped_autorelease_pool.h"

// MacCocoaSocketServerHelperSm serves as a delegate to NSMachPort or a target for
// a timeout.
@interface MacCocoaSocketServerHelperSm : NSObject {
  // This is a weak reference. This works fine since the
  // MacCocoaSocketServer owns this object.
  MacCocoaSocketServer* socketServer_;  // Weak.
}
@end

@implementation MacCocoaSocketServerHelperSm
- (id)initWithSocketServer:(MacCocoaSocketServer*)ss {
  self = [super init];
  if (self) {
    socketServer_ = ss;
  }
  return self;
}

- (void)timerFired:(NSTimer*)timer {
  socketServer_->WakeUp();
}

- (void)breakMainloop {
  [NSApp stop:self];
  // NSApp stop only exits after finishing processing of the
  // current event.  Since we're potentially in a timer callback
  // and not an NSEvent handler, we need to trigger a dummy one
  // and turn the loop over.  We may be able to skip this if we're
  // on the ss' thread and not inside the app loop already.
  NSEvent* event = [NSEvent otherEventWithType:NSApplicationDefined
                                      location:NSMakePoint(0,0)
                                 modifierFlags:0
                                     timestamp:0
                                  windowNumber:0
                                       context:nil
                                       subtype:0
                                         data1:0
                                         data2:0];
  [NSApp postEvent:event atStart:NO];
}
@end

namespace sm {

MacCocoaSocketServer::MacCocoaSocketServer() {
  helper_ = [[MacCocoaSocketServerHelperSm alloc] initWithSocketServer:this];
  timer_ = nil;
  run_count_ = 0;

  // Initialize the shared NSApplication
  [NSApplication sharedApplication];
}

MacCocoaSocketServer::~MacCocoaSocketServer() {
  [timer_ invalidate];
  [timer_ release];
  [helper_ release];
}

// ::Wait is reentrant, for example when blocking on another thread while
// responding to I/O. Calls to [NSApp] MUST be made from the main thread
// only!
bool MacCocoaSocketServer::Wait(int cms, bool process_io) {
  ScopedAutoreleasePool pool;
  if (!process_io && cms == 0) {
    // No op.
    return true;
  }
  if ([NSApp isRunning]) {
    // Only allow reentrant waiting if we're in a blocking send.
    ASSERT(!process_io && cms == kForever);
  }

  if (!process_io) {
    // No way to listen to common modes and not get socket events, unless
    // we disable each one's callbacks.
    EnableSocketCallbacks(false);
  }

  if (kForever != cms) {
    // Install a timer that fires wakeup after cms has elapsed.
    timer_ =
        [NSTimer scheduledTimerWithTimeInterval:cms / 1000.0
                                         target:helper_
                                       selector:@selector(timerFired:)
                                       userInfo:nil
                                        repeats:NO];
    [timer_ retain];
  }

  // Run until WakeUp is called, which will call stop and exit this loop.
  run_count_++;
  [NSApp run];
  run_count_--;

  if (!process_io) {
    // Reenable them.  Hopefully this won't cause spurious callbacks or
    // missing ones while they were disabled.
    EnableSocketCallbacks(true);
  }

  return true;
}

// Can be called from any thread.  Post a message back to the main thread to
// break out of the NSApp loop.
void MacCocoaSocketServer::WakeUp() {
  if (timer_ != nil) {
    [timer_ invalidate];
    [timer_ release];
    timer_ = nil;
  }

  // [NSApp isRunning] returns unexpected results when called from another
  // thread.  Maintain our own count of how many times to break the main loop.
  if (run_count_ > 0) {
    [helper_ performSelectorOnMainThread:@selector(breakMainloop)
                              withObject:nil
                           waitUntilDone:false];
  }
}

}  // namespace sm
