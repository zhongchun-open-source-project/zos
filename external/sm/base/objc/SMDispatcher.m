// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#import "sm/base/objc/SMDispatcher.h"

static dispatch_queue_t kCaptureSessionQueue = nil;

@implementation SMDispatcher {
  dispatch_queue_t _captureSessionQueue;
}

+ (void)initialize {
  static dispatch_once_t onceToken;
  dispatch_once(&onceToken, ^{
    kCaptureSessionQueue = dispatch_queue_create(
        "com.zhongchun.sm.SMDispatcherCaptureSession",
        DISPATCH_QUEUE_SERIAL);
  });
}

+ (void)dispatchAsyncOnType:(SMDispatcherQueueType)dispatchType
                      block:(dispatch_block_t)block {
  dispatch_queue_t queue = [self dispatchQueueForType:dispatchType];
  dispatch_async(queue, block);
}

#pragma mark - Private

+ (dispatch_queue_t)dispatchQueueForType:(SMDispatcherQueueType)dispatchType {
  switch (dispatchType) {
    case SMDispatcherTypeMain:
      return dispatch_get_main_queue();
    case SMDispatcherTypeCaptureSession:
      return kCaptureSessionQueue;
  }
}

@end

