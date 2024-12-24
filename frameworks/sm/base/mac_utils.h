// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_MAC_UTILS_H_
#define SM_BASE_MAC_UTILS_H_

#include <CoreFoundation/CoreFoundation.h>
#if defined(SM_MAC) && !defined(SM_IOS)
#include <Carbon/Carbon.h>
#endif
#include <string>

namespace sm {

///////////////////////////////////////////////////////////////////////////////

// Note that some of these functions work for both iOS and Mac OS X.  The ones
// that are specific to Mac are #ifdef'ed as such.

bool ToUtf8(const CFStringRef str16, std::string* str8);
bool ToUtf16(const std::string& str8, CFStringRef* str16);

#if defined(SM_MAC) && !defined(SM_IOS)
void DecodeFourChar(UInt32 fc, std::string* out);

enum MacOSVersionName {
  kMacOSUnknown,       // ???
  kMacOSOlder,         // 10.2-
  kMacOSPanther,       // 10.3
  kMacOSTiger,         // 10.4
  kMacOSLeopard,       // 10.5
  kMacOSSnowLeopard,   // 10.6
  kMacOSLion,          // 10.7
  kMacOSMountainLion,  // 10.8
  kMacOSMavericks,     // 10.9
  kMacOSNewer,         // 10.10+
};

bool GetOSVersion(int* major, int* minor, int* bugfix);
MacOSVersionName GetOSVersionName();
bool GetQuickTimeVersion(std::string* version);

// Runs the given apple script. Only supports scripts that does not
// require user interaction.
bool RunAppleScript(const std::string& script);
#endif

///////////////////////////////////////////////////////////////////////////////

}  // namespace sm

#endif  // SM_BASE_MAC_UTILS_H_
