// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_LINUX_FD_WALK_H_
#define SM_BASE_LINUX_FD_WALK_H_

#ifdef __cplusplus
extern "C" {
#endif

// Linux port of SunOS's fdwalk(3) call. It loops over all open file descriptors
// and calls func on each one. Additionally, it is safe to use from the child
// of a fork that hasn't exec'ed yet, so you can use it to close all open file
// descriptors prior to exec'ing a daemon.
// The return value is 0 if successful, or else -1 and errno is set. The
// possible errors include any error that can be returned by opendir(),
// readdir(), or closedir(), plus EBADF if there are problems parsing the
// contents of /proc/self/fd.
// The file descriptors that are enumerated will not include the file descriptor
// used for the enumeration itself.
int fdwalk(void (*func)(void *, int), void *opaque);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // SM_BASE_LINUX_FD_WALK_H_
