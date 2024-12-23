// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_FIELD_TRIAL_H_
#define SM_BASE_FIELD_TRIAL_H_

#include <string>

// Field trials allow sm clients (such as Chrome) to turn on feature code
// in binaries out in the field and gather information with that.
//
// WebRTC clients MUST provide an implementation of:
//
//   std::string field_trial::FindFullName(const std::string& trial).
//
// Or link with a default one provided in:
//
//   system_wrappers/system_wrappers.gyp:field_trial_default
//
//
// They are designed to wire up directly to chrome field trials and to speed up
// developers by reducing the need to wire APIs to control whether a feature is
// on/off. E.g. to experiment with a new method that could lead to a different
// trade-off between CPU/bandwidth:
//
// 1 - Develop the feature with default behaviour off:
//
//   if (FieldTrial::FindFullName("WebRTCExperimenMethod2") == "Enabled")
//     method2();
//   else
//     method1();
//
// 2 - Once the changes are rolled to chrome, the new code path can be
//     controlled as normal chrome field trials.
//
// 3 - Evaluate the new feature and clean the code paths.
//
// Notes:
//   - NOT every feature is a candidate to be controlled by this mechanism as
//     it may require negotation between involved parties (e.g. SDP).
//
// TODO(andresp): since chrome --force-fieldtrials does not marks the trial
//     as active it does not gets propaged to renderer process. For now one
//     needs to push a config with start_active:true or run a local finch
//     server.
//
// TODO(andresp): find out how to get bots to run tests with trials enabled.

namespace sm {
namespace field_trial {

// Returns the group name chosen for the named trial, or the empty string
// if the trial does not exists.
//
// Note: To keep things tidy append all the trial names with WebRTC.
std::string FindFullName(const std::string& name);

}  // namespace field_trial
}  // namespace sm

#endif  // SM_BASE_FIELD_TRIAL_H_
