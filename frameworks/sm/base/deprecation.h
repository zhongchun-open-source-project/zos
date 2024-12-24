// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_DEPRECATION_H_
#define SM_BASE_DEPRECATION_H_

// Annotate the declarations of deprecated functions with this to cause a
// compiler warning when they're used. Like so:
//
//   SM_DEPRECATED std::pony PonyPlz(const std::pony_spec& ps);
//
// NOTE 1: The annotation goes on the declaration in the .h file, not the
// definition in the .cc file!
//
// NOTE 2: In order to keep unit testing the deprecated function without
// getting warnings, do something like this:
//
//   std::pony DEPRECATED_PonyPlz(const std::pony_spec& ps);
//   SM_DEPRECATED inline std::pony PonyPlz(const std::pony_spec& ps) {
//     return DEPRECATED_PonyPlz(ps);
//   }
//
// In other words, rename the existing function, and provide an inline wrapper
// using the original name that calls it. That way, callers who are willing to
// call it using the DEPRECATED_-prefixed name don't get the warning.
//
// TODO(kwiberg): Remove this when we can use [[deprecated]] from C++14.
#if defined(_MSC_VER)
// Note: Deprecation warnings seem to fail to trigger on Win32
// (https://bugs.chromium.org/p/sm/issues/detail?id=5368).
#define SM_DEPRECATED __declspec(deprecated)
#elif defined(__GNUC__)
#define SM_DEPRECATED __attribute__ ((__deprecated__))
#else
#define SM_DEPRECATED
#endif

#endif  // SM_BASE_DEPRECATION_H_
