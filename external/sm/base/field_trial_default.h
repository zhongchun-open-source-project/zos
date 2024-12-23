// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_FIELD_TRIAL_DEFAULT_H_
#define SM_BASE_FIELD_TRIAL_DEFAULT_H_

namespace sm {
namespace field_trial {

// Optionally initialize field trial from a string.
// This method can be called at most once before any other call into sm.
// E.g. before the peer connection factory is constructed.
// Note: trials_string must never be destroyed.
void InitFieldTrialsFromString(const char* trials_string);

const char* GetFieldTrialString();

}  // namespace field_trial
}  // namespace sm

#endif  // SM_BASE_FIELD_TRIAL_DEFAULT_H_
