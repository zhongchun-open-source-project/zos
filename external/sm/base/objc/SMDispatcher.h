// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, SMDispatcherQueueType) {
  // Main dispatcher queue.
  SMDispatcherTypeMain,
  // Used for starting/stopping AVCaptureSession, and assigning
  // capture session to AVCaptureVideoPreviewLayer.
  SMDispatcherTypeCaptureSession,
};

/** Dispatcher that asynchronously dispatches blocks to a specific
 *  shared dispatch queue.
 */
@interface SMDispatcher : NSObject

- (instancetype)init NS_UNAVAILABLE;

/** Dispatch the block asynchronously on the queue for dispatchType.
 *  @param dispatchType The queue type to dispatch on.
 *  @param block The block to dispatch asynchronously.
 */
+ (void)dispatchAsyncOnType:(SMDispatcherQueueType)dispatchType
                      block:(dispatch_block_t)block;

@end
