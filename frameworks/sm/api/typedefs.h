// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

// This file contains platform-specific typedefs and defines.

#ifndef SM_TYPEDEFS_H_
#define SM_TYPEDEFS_H_

// Processor architecture detection.  For more info on what's defined, see:
//   http://msdn.microsoft.com/en-us/library/b0084kay.aspx
//   http://www.agner.org/optimize/calling_conventions.pdf
//   or with gcc, run: "echo | gcc -E -dM -"
#if defined(_M_X64) || defined(__x86_64__)
#define SM_ARCH_X86_FAMILY
#define SM_ARCH_X86_64
#define SM_ARCH_64_BITS
#define SM_ARCH_LITTLE_ENDIAN
#elif defined(__aarch64__)
#define SM_ARCH_ARM_FAMILY
#define SM_ARCH_64_BITS
#define SM_ARCH_LITTLE_ENDIAN
#elif defined(_M_IX86) || defined(__i386__)
#define SM_ARCH_X86_FAMILY
#define SM_ARCH_X86
#define SM_ARCH_32_BITS
#define SM_ARCH_LITTLE_ENDIAN
#elif defined(__ARMEL__)
#define SM_ARCH_ARM_FAMILY
#define SM_ARCH_32_BITS
#define SM_ARCH_LITTLE_ENDIAN
#elif defined(__MIPSEL__)
#if defined(__LP64__)
#define SM_ARCH_MIPS64_FAMILY
#else
#define SM_ARCH_MIPS_FAMILY
#endif
#define SM_ARCH_32_BITS
#define SM_ARCH_LITTLE_ENDIAN
#elif defined(__pnacl__)
#define SM_ARCH_32_BITS
#define SM_ARCH_LITTLE_ENDIAN
#else
#error Please add support for your architecture in typedefs.h
#endif

#if !(defined(SM_ARCH_LITTLE_ENDIAN) ^ defined(SM_ARCH_BIG_ENDIAN))
#error Define either SM_ARCH_LITTLE_ENDIAN or SM_ARCH_BIG_ENDIAN
#endif

// TODO(zhongwei.yao): SM_CPU_DETECTION is only used in one place; we should
// probably just remove it.
#if (defined(SM_ARCH_X86_FAMILY) && !defined(__SSE2__)) || \
    defined(SM_DETECT_NEON)
#define SM_CPU_DETECTION
#endif

// TODO(pbos): Use sm/base/basictypes.h instead to include fixed-size ints.
#include <stdint.h>

// Annotate a function indicating the caller must examine the return value.
// Use like:
//   int foo() WARN_UNUSED_RESULT;
// To explicitly ignore a result, see |ignore_result()| in <base/macros.h>.
// TODO(ajm): Hack to avoid multiple definitions until the base/ of sm and
// libsm are merged.
#if !defined(WARN_UNUSED_RESULT)
#if defined(__GNUC__) || defined(__clang__)
#define WARN_UNUSED_RESULT __attribute__ ((__warn_unused_result__))
#else
#define WARN_UNUSED_RESULT
#endif
#endif  // WARN_UNUSED_RESULT

// Put after a variable that might not be used, to prevent compiler warnings:
//   int result ATTRIBUTE_UNUSED = DoSomething();
//   assert(result == 17);
#ifndef ATTRIBUTE_UNUSED
#if defined(__GNUC__) || defined(__clang__)
#define ATTRIBUTE_UNUSED __attribute__ ((__unused__))
#else
#define ATTRIBUTE_UNUSED
#endif
#endif

// Macro to be used for switch-case fallthrough (required for enabling
// -Wimplicit-fallthrough warning on Clang).
#ifndef FALLTHROUGH
#if defined(__clang__)
#define FALLTHROUGH() [[clang::fallthrough]]
#else
#define FALLTHROUGH() do { } while (0)
#endif
#endif

// Annotate a function that will not return control flow to the caller.
#if defined(_MSC_VER)
#define NO_RETURN __declspec(noreturn)
#elif defined(__GNUC__)
#define NO_RETURN __attribute__ ((__noreturn__))
#else
#define NO_RETURN
#endif

#endif  // SM_TYPEDEFS_H_