// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

// Helper function for using Cocoa with Posix threads. This header should be
// included from C/C++ files that want to use some Cocoa functionality without
// using the .mm extension (mostly for files that are compiled on multiple
// platforms).

#ifndef SM_BASE_MAC_COCOA_THREAD_HELPER_H_
#define SM_BASE_MAC_COCOA_THREAD_HELPER_H_

namespace sm {

// Cocoa must be "put into multithreading mode" before Cocoa functionality can
// be used on POSIX threads. This function does that.
void InitCocoaMultiThreading();

}  // namespace sm

#endif  // SM_BASE_MAC_COCOA_THREAD_HELPER_H_
