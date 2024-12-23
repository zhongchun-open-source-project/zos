// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_FORMAT_MACROS_H_
#define SM_BASE_FORMAT_MACROS_H_

// This file defines the format macros for some integer types and is derived
// from Chromium's base/format_macros.h.

// To print a 64-bit value in a portable way:
//   int64_t value;
//   printf("xyz:%" PRId64, value);
// The "d" in the macro corresponds to %d; you can also use PRIu64 etc.
//
// To print a size_t value in a portable way:
//   size_t size;
//   printf("xyz: %" PRIuS, size);
// The "u" in the macro corresponds to %u, and S is for "size".

#include "sm/typedefs.h"

#if defined(SM_POSIX)

#if (defined(_INTTYPES_H) || defined(_INTTYPES_H_)) && !defined(PRId64)
#error "inttypes.h has already been included before this header file, but "
#error "without __STDC_FORMAT_MACROS defined."
#endif

#if !defined(__STDC_FORMAT_MACROS)
#define __STDC_FORMAT_MACROS
#endif

#include <inttypes.h>

#if !defined(PRIuS)
#define PRIuS "zu"
#endif

// The size of NSInteger and NSUInteger varies between 32-bit and 64-bit
// architectures and Apple does not provides standard format macros and
// recommends casting. This has many drawbacks, so instead define macros
// for formatting those types.
#if defined(SM_MAC)
#if defined(SM_ARCH_64_BITS)
#if !defined(PRIdNS)
#define PRIdNS "ld"
#endif
#if !defined(PRIuNS)
#define PRIuNS "lu"
#endif
#if !defined(PRIxNS)
#define PRIxNS "lx"
#endif
#else  // defined(SM_ARCH_64_BITS)
#if !defined(PRIdNS)
#define PRIdNS "d"
#endif
#if !defined(PRIuNS)
#define PRIuNS "u"
#endif
#if !defined(PRIxNS)
#define PRIxNS "x"
#endif
#endif
#endif  // defined(SM_MAC)

#else  // SM_WIN

#include <inttypes.h>

#if !defined(PRId64)
#define PRId64 "I64d"
#endif

#if !defined(PRIu64)
#define PRIu64 "I64u"
#endif

#if !defined(PRIx64)
#define PRIx64 "I64x"
#endif

#if !defined(PRIuS)
#define PRIuS "Iu"
#endif

#endif

#endif  // SM_BASE_FORMAT_MACROS_H_
