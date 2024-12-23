// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class AVCaptureSession;
@class SMAVFoundationVideoSource;

/** SM_CameraPreviewView is a view that renders local video from an
 *  AVCaptureSession.
 */
@interface SMCameraPreviewView : UIView

/** The capture session being rendered in the view. Capture session
 *  is assigned to AVCaptureVideoPreviewLayer async in the same
 *  queue that the AVCaptureSession is started/stopped.
 */
@property(nonatomic, strong) AVCaptureSession *captureSession;

@end
