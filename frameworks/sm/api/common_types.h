// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_COMMON_TYPES_H_
#define SM_COMMON_TYPES_H_

#include <stddef.h>
#include <string.h>

#include <string>
#include <vector>

#include "sm/typedefs.h"

#if defined(_MSC_VER)
// Disable "new behavior: elements of array will be default initialized"
// warning. Affects OverUseDetectorOptions.
#pragma warning(disable:4351)
#endif

#ifdef SM_EXPORT
#define SM_DLLEXPORT _declspec(dllexport)
#elif SM_DLL
#define SM_DLLEXPORT _declspec(dllimport)
#else
#define SM_DLLEXPORT
#endif

#ifndef NULL
#define NULL 0
#endif

#define RTP_PAYLOAD_NAME_SIZE 32

#if defined(SM_WIN) || defined(WIN32)
// Compares two strings without regard to case.
#define STR_CASE_CMP(s1, s2) ::_stricmp(s1, s2)
// Compares characters of two strings without regard to case.
#define STR_NCASE_CMP(s1, s2, n) ::_strnicmp(s1, s2, n)
#else
#define STR_CASE_CMP(s1, s2) ::strcasecmp(s1, s2)
#define STR_NCASE_CMP(s1, s2, n) ::strncasecmp(s1, s2, n)
#endif

namespace sm {

class Config;


}  // namespace sm

#endif  // SM_COMMON_TYPES_H_