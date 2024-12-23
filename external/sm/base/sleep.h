// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

// An OS-independent sleep function.

#ifndef SM_BASE_SLEEP_H_
#define SM_BASE_SLEEP_H_

namespace sm {

// This function sleeps for the specified number of milliseconds.
// It may return early if the thread is woken by some other event,
// such as the delivery of a signal on Unix.
void SleepMs(int msecs);

}  // namespace sm

#endif  // SM_BASE_SLEEP_H_
