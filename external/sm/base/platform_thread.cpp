// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/platform_thread.h"

#include "sm/base/checks.h"

#if defined(SM_LINUX)
#include <sys/prctl.h>
#include <sys/syscall.h>
#endif

namespace sm {

PlatformThreadId CurrentThreadId() {
  PlatformThreadId ret;
#if defined(SM_WIN)
  ret = GetCurrentThreadId();
#elif defined(SM_POSIX)
#if defined(SM_MAC) || defined(SM_IOS)
  ret = pthread_mach_thread_np(pthread_self());
#elif defined(SM_LINUX)
  ret =  syscall(__NR_gettid);
#elif defined(SM_ANDROID)
  ret = gettid();
#else
  // Default implementation for nacl and solaris.
  ret = reinterpret_cast<pid_t>(pthread_self());
#endif
#endif  // defined(SM_POSIX)
  SM_DCHECK(ret);
  return ret;
}

PlatformThreadRef CurrentThreadRef() {
#if defined(SM_WIN)
  return GetCurrentThreadId();
#elif defined(SM_POSIX)
  return pthread_self();
#endif
}

bool IsThreadRefEqual(const PlatformThreadRef& a, const PlatformThreadRef& b) {
#if defined(SM_WIN)
  return a == b;
#elif defined(SM_POSIX)
  return pthread_equal(a, b);
#endif
}

void SetCurrentThreadName(const char* name) {
#if defined(SM_WIN)
  struct {
    DWORD dwType;
    LPCSTR szName;
    DWORD dwThreadID;
    DWORD dwFlags;
  } threadname_info = {0x1000, name, static_cast<DWORD>(-1), 0};

  __try {
    ::RaiseException(0x406D1388, 0, sizeof(threadname_info) / sizeof(DWORD),
                     reinterpret_cast<ULONG_PTR*>(&threadname_info));
  } __except (EXCEPTION_EXECUTE_HANDLER) {
  }
#elif defined(SM_LINUX) || defined(SM_ANDROID)
  prctl(PR_SET_NAME, reinterpret_cast<unsigned long>(name));
#elif defined(SM_MAC) || defined(SM_IOS)
  pthread_setname_np(name);
#endif
}

namespace {
#if defined(SM_WIN)
void CALLBACK RaiseFlag(ULONG_PTR param) {
  *reinterpret_cast<bool*>(param) = true;
}
#else
struct ThreadAttributes {
  ThreadAttributes() { pthread_attr_init(&attr); }
  ~ThreadAttributes() { pthread_attr_destroy(&attr); }
  pthread_attr_t* operator&() { return &attr; }
  pthread_attr_t attr;
};
#endif  // defined(SM_WIN)
}

PlatformThread::PlatformThread(ThreadRunFunction func,
                               void* obj,
                               const char* thread_name)
    : run_function_(func),
      obj_(obj),
      name_(thread_name ? thread_name : "sm"),
#if defined(SM_WIN)
      stop_(false),
      thread_(NULL) {
#else
      stop_event_(false, false),
      thread_(0) {
#endif  // defined(SM_WIN)
  SM_DCHECK(func);
  SM_DCHECK(name_.length() < 64);
}

PlatformThread::~PlatformThread() {
  SM_DCHECK(thread_checker_.CalledOnValidThread());
#if defined(SM_WIN)
  SM_DCHECK(!thread_);
#endif  // defined(SM_WIN)
}

#if defined(SM_WIN)
DWORD WINAPI PlatformThread::StartThread(void* param) {
  static_cast<PlatformThread*>(param)->Run();
  return 0;
}
#else
void* PlatformThread::StartThread(void* param) {
  static_cast<PlatformThread*>(param)->Run();
  return 0;
}
#endif  // defined(SM_WIN)

void PlatformThread::Start() {
  SM_DCHECK(thread_checker_.CalledOnValidThread());
  SM_DCHECK(!thread_) << "Thread already started?";
#if defined(SM_WIN)
  stop_ = false;

  // See bug 2902 for background on STACK_SIZE_PARAM_IS_A_RESERVATION.
  // Set the reserved stack stack size to 1M, which is the default on Win32
  // and Linux.
  DWORD thread_id;
  thread_ = ::CreateThread(NULL, 1024 * 1024, &StartThread, this,
                           STACK_SIZE_PARAM_IS_A_RESERVATION, &thread_id);
  SM_CHECK(thread_) << "CreateThread failed";
#else
  ThreadAttributes attr;
  // Set the stack stack size to 1M.
  pthread_attr_setstacksize(&attr, 1024 * 1024);
  SM_CHECK_EQ(0, pthread_create(&thread_, &attr, &StartThread, this));
#endif  // defined(SM_WIN)
}

bool PlatformThread::IsRunning() const {
  SM_DCHECK(thread_checker_.CalledOnValidThread());
#if defined(SM_WIN)
  return thread_ != nullptr;
#else
  return thread_ != 0;
#endif  // defined(SM_WIN)
}

void PlatformThread::Stop() {
  SM_DCHECK(thread_checker_.CalledOnValidThread());
  if (!IsRunning())
    return;

#if defined(SM_WIN)
  // Set stop_ to |true| on the worker thread.
  QueueUserAPC(&RaiseFlag, thread_, reinterpret_cast<ULONG_PTR>(&stop_));
  WaitForSingleObject(thread_, INFINITE);
  CloseHandle(thread_);
  thread_ = nullptr;
#else
  stop_event_.Set();
  SM_CHECK_EQ(0, pthread_join(thread_, nullptr));
  thread_ = 0;
#endif  // defined(SM_WIN)
}

void PlatformThread::Run() {
  if (!name_.empty())
    SetCurrentThreadName(name_.c_str());
  do {
    // The interface contract of Start/Stop is that for a successfull call to
    // Start, there should be at least one call to the run function.  So we
    // call the function before checking |stop_|.
    if (!run_function_(obj_))
      break;
#if defined(SM_WIN)
    // Alertable sleep to permit RaiseFlag to run and update |stop_|.
    SleepEx(0, true);
  } while (!stop_);
#else
  } while (!stop_event_.Wait(0));
#endif  // defined(SM_WIN)
}

bool PlatformThread::SetPriority(ThreadPriority priority) {
  SM_DCHECK(thread_checker_.CalledOnValidThread());
  SM_DCHECK(IsRunning());
#if defined(SM_WIN)
  return SetThreadPriority(thread_, priority) != FALSE;
#elif defined(__native_client__)
  // Setting thread priorities is not supported in NaCl.
  return true;
#elif defined(SM_CHROMIUM_BUILD) && defined(SM_LINUX)
  // TODO(tommi): Switch to the same mechanism as Chromium uses for changing
  // thread priorities.
  return true;
#else
#ifdef SM_THREAD_RR
  const int policy = SCHED_RR;
#else
  const int policy = SCHED_FIFO;
#endif
  const int min_prio = sched_get_priority_min(policy);
  const int max_prio = sched_get_priority_max(policy);
  if (min_prio == -1 || max_prio == -1) {
    return false;
  }

  if (max_prio - min_prio <= 2)
    return false;

  // Convert sm priority to system priorities:
  sched_param param;
  const int top_prio = max_prio - 1;
  const int low_prio = min_prio + 1;
  switch (priority) {
    case kLowPriority:
      param.sched_priority = low_prio;
      break;
    case kNormalPriority:
      // The -1 ensures that the kHighPriority is always greater or equal to
      // kNormalPriority.
      param.sched_priority = (low_prio + top_prio - 1) / 2;
      break;
    case kHighPriority:
      param.sched_priority = std::max(top_prio - 2, low_prio);
      break;
    case kHighestPriority:
      param.sched_priority = std::max(top_prio - 1, low_prio);
      break;
    case kRealtimePriority:
      param.sched_priority = top_prio;
      break;
  }
  return pthread_setschedparam(thread_, policy, &param) == 0;
#endif  // defined(SM_WIN)
}

}  // namespace sm
