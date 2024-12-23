// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

#import "sm/base/objc/SMCameraPreviewView.h"

#import <AVFoundation/AVFoundation.h>

#import "sm/base/objc/SMDispatcher.h"

@implementation SMCameraPreviewView

@synthesize captureSession = _captureSession;

+ (Class)layerClass {
  return [AVCaptureVideoPreviewLayer class];
}

- (void)setCaptureSession:(AVCaptureSession *)captureSession {
  if (_captureSession == captureSession) {
    return;
  }
  _captureSession = captureSession;
  AVCaptureVideoPreviewLayer *previewLayer = [self previewLayer];
  [SMDispatcher dispatchAsyncOnType:SMDispatcherTypeCaptureSession
                               block:^{
    previewLayer.session = captureSession;
  }];
}

#pragma mark - Private

- (AVCaptureVideoPreviewLayer *)previewLayer {
  return (AVCaptureVideoPreviewLayer *)self.layer;
}

@end
