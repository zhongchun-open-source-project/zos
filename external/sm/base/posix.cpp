// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/posix.h"

#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

#if defined(SM_LINUX) && !defined(SM_ANDROID)
#include "sm/base/x11_fd_walk.h"
#endif
#include "sm/base/logging.h"

namespace sm {

#if defined(SM_LINUX) && !defined(SM_ANDROID)
static void closefds(void *close_errors, int fd) {
  if (fd <= 2) {
    // We leave stdin/out/err open to the browser's terminal, if any.
    return;
  }
  if (close(fd) < 0) {
    *static_cast<bool *>(close_errors) = true;
  }
}
#endif

enum {
  EXIT_FLAG_CHDIR_ERRORS       = 1 << 0,
#if defined(SM_LINUX) && !defined(SM_ANDROID)
  EXIT_FLAG_FDWALK_ERRORS      = 1 << 1,
  EXIT_FLAG_CLOSE_ERRORS       = 1 << 2,
#endif
  EXIT_FLAG_SECOND_FORK_FAILED = 1 << 3,
};

bool RunAsDaemon(const char *file, const char *const argv[]) {
  // Fork intermediate child to daemonize.
  pid_t pid = fork();
  if (pid < 0) {
    LOG_ERR(LS_ERROR) << "fork()";
    return false;
  } else if (!pid) {
    // Child.

    // We try to close all fds and change directory to /, but if that fails we
    // keep going because it's not critical.
    int exit_code = 0;
    if (chdir("/") < 0) {
      exit_code |= EXIT_FLAG_CHDIR_ERRORS;
    }
#if defined(SM_LINUX) && !defined(SM_ANDROID)
    bool close_errors = false;
    if (fdwalk(&closefds, &close_errors) < 0) {
      exit_code |= EXIT_FLAG_FDWALK_ERRORS;
    }
    if (close_errors) {
      exit_code |= EXIT_FLAG_CLOSE_ERRORS;
    }
#endif

    // Fork again to become a daemon.
    pid = fork();
    // It is important that everything here use _exit() and not exit(), because
    // exit() would call the destructors of all global variables in the whole
    // process, which is both unnecessary and unsafe.
    if (pid < 0) {
      exit_code |= EXIT_FLAG_SECOND_FORK_FAILED;
      _exit(exit_code);  // if second fork failed
    } else if (!pid) {
      // Child.
      // Successfully daemonized. Run command.
      // SM_POSIX requires the args to be typed as non-const for historical
      // reasons, but it mandates that the actual implementation be const, so
      // the cast is safe.
      execvp(file, const_cast<char *const *>(argv));
      _exit(255);  // if execvp failed
    }

    // Parent.
    // Successfully spawned process, but report any problems to the parent where
    // we can log them.
    _exit(exit_code);
  }

  // Parent. Reap intermediate child.
  int status;
  pid_t child = waitpid(pid, &status, 0);
  if (child < 0) {
    LOG_ERR(LS_ERROR) << "Error in waitpid()";
    return false;
  }
  if (child != pid) {
    // Should never happen (see man page).
    LOG(LS_ERROR) << "waitpid() chose wrong child???";
    return false;
  }
  if (!WIFEXITED(status)) {
    LOG(LS_ERROR) << "Intermediate child killed uncleanly";  // Probably crashed
    return false;
  }

  int exit_code = WEXITSTATUS(status);
  if (exit_code & EXIT_FLAG_CHDIR_ERRORS) {
    LOG(LS_WARNING) << "Child reported probles calling chdir()";
  }
#if defined(SM_LINUX) && !defined(SM_ANDROID)
  if (exit_code & EXIT_FLAG_FDWALK_ERRORS) {
    LOG(LS_WARNING) << "Child reported problems calling fdwalk()";
  }
  if (exit_code & EXIT_FLAG_CLOSE_ERRORS) {
    LOG(LS_WARNING) << "Child reported problems calling close()";
  }
#endif
  if (exit_code & EXIT_FLAG_SECOND_FORK_FAILED) {
    LOG(LS_ERROR) << "Failed to daemonize";
    // This means the command was not launched, so failure.
    return false;
  }
  return true;
}

}  // namespace sm
