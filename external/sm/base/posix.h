// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_POSIX_H_
#define SM_BASE_POSIX_H_

namespace sm {

// Runs the given executable name as a daemon, so that it executes concurrently
// with this process. Upon completion, the daemon process will automatically be
// reaped by init(8), so an error exit status or a failure to start the
// executable are not reported. Returns true if the daemon process was forked
// successfully, else false.
bool RunAsDaemon(const char *file, const char *const argv[]);

}  // namespace sm

#endif  // SM_BASE_POSIX_H_
