// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/condition_variable_event_win.h"
#include "sm/base/critical_section_win.h"

namespace sm {

ConditionVariableEventWin::ConditionVariableEventWin() : eventID_(WAKEALL_0) {
  memset(&num_waiters_[0], 0, sizeof(num_waiters_));

  InitializeCriticalSection(&num_waiters_crit_sect_);

  events_[WAKEALL_0] = CreateEvent(NULL,  // no security attributes
                                   TRUE,  // manual-reset, sticky event
                                   FALSE,  // initial state non-signaled
                                   NULL);  // no name for event

  events_[WAKEALL_1] = CreateEvent(NULL,  // no security attributes
                                   TRUE,  // manual-reset, sticky event
                                   FALSE,  // initial state non-signaled
                                   NULL);  // no name for event

  events_[WAKE] = CreateEvent(NULL,  // no security attributes
                              FALSE,  // auto-reset, sticky event
                              FALSE,  // initial state non-signaled
                              NULL);  // no name for event
}

ConditionVariableEventWin::~ConditionVariableEventWin() {
  CloseHandle(events_[WAKE]);
  CloseHandle(events_[WAKEALL_1]);
  CloseHandle(events_[WAKEALL_0]);

  DeleteCriticalSection(&num_waiters_crit_sect_);
}

void ConditionVariableEventWin::SleepCS(CriticalSectionWrapper& crit_sect) {
  SleepCS(crit_sect, INFINITE);
}

bool ConditionVariableEventWin::SleepCS(CriticalSectionWrapper& crit_sect,
                                        unsigned long max_time_in_ms) {
  EnterCriticalSection(&num_waiters_crit_sect_);

  // Get the eventID for the event that will be triggered by next
  // WakeAll() call and start waiting for it.
  const EventWakeUpType eventID =
      (WAKEALL_0 == eventID_) ? WAKEALL_1 : WAKEALL_0;

  ++(num_waiters_[eventID]);
  LeaveCriticalSection(&num_waiters_crit_sect_);

  CriticalSectionWindows* cs =
      static_cast<CriticalSectionWindows*>(&crit_sect);
  LeaveCriticalSection(&cs->crit);
  HANDLE events[2];
  events[0] = events_[WAKE];
  events[1] = events_[eventID];
  const DWORD result = WaitForMultipleObjects(2,  // Wait on 2 events.
                                              events,
                                              FALSE,  // Wait for either.
                                              max_time_in_ms);

  const bool ret_val = (result != WAIT_TIMEOUT);

  EnterCriticalSection(&num_waiters_crit_sect_);
  --(num_waiters_[eventID]);

  // Last waiter should only be true for WakeAll(). WakeAll() correspond
  // to position 1 in events[] -> (result == WAIT_OBJECT_0 + 1)
  const bool last_waiter = (result == WAIT_OBJECT_0 + 1) &&
      (num_waiters_[eventID] == 0);
  LeaveCriticalSection(&num_waiters_crit_sect_);

  if (last_waiter) {
    // Reset/unset the WakeAll() event since all threads have been
    // released.
    ResetEvent(events_[eventID]);
  }

  EnterCriticalSection(&cs->crit);
  return ret_val;
}

void ConditionVariableEventWin::Wake() {
  EnterCriticalSection(&num_waiters_crit_sect_);
  const bool have_waiters = (num_waiters_[WAKEALL_0] > 0) ||
      (num_waiters_[WAKEALL_1] > 0);
  LeaveCriticalSection(&num_waiters_crit_sect_);

  if (have_waiters) {
    SetEvent(events_[WAKE]);
  }
}

void ConditionVariableEventWin::WakeAll() {
  EnterCriticalSection(&num_waiters_crit_sect_);

  // Update current WakeAll() event
  eventID_ = (WAKEALL_0 == eventID_) ? WAKEALL_1 : WAKEALL_0;

  // Trigger current event
  const EventWakeUpType eventID = eventID_;
  const bool have_waiters = num_waiters_[eventID] > 0;
  LeaveCriticalSection(&num_waiters_crit_sect_);

  if (have_waiters) {
    SetEvent(events_[eventID]);
  }
}

}  // namespace sm
