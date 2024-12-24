// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_BASIC_TYPES_H_
#define SM_BASE_BASIC_TYPES_H_

#include <stddef.h>  // for NULL, size_t
#include <stdint.h>  // for uintptr_t and (u)int_t types.

#ifdef HAVE_CONFIG_H
#include "config.h"  // NOLINT
#endif

// Detect compiler is for x86 or x64.
#if defined(__x86_64__) || defined(_M_X64) || \
    defined(__i386__) || defined(_M_IX86)
#define CPU_X86 1
#endif

// Detect compiler is for arm.
#if defined(__arm__) || defined(_M_ARM)
#define CPU_ARM 1
#endif

#if defined(CPU_X86) && defined(CPU_ARM)
#error CPU_X86 and CPU_ARM both defined.
#endif

#if !defined(SM_ARCH_CPU_BIG_ENDIAN) && !defined(SM_ARCH_CPU_LITTLE_ENDIAN)
// x86, arm or GCC provided __BYTE_ORDER__ macros
#if CPU_X86 || CPU_ARM ||  \
  (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define SM_ARCH_CPU_LITTLE_ENDIAN
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define SM_ARCH_CPU_BIG_ENDIAN
#else
#error SM_ARCH_CPU_BIG_ENDIAN or SM_ARCH_CPU_LITTLE_ENDIAN should be defined.
#endif
#endif

#if defined(SM_ARCH_CPU_BIG_ENDIAN) && defined(SM_ARCH_CPU_LITTLE_ENDIAN)
#error SM_ARCH_CPU_BIG_ENDIAN and SM_ARCH_CPU_LITTLE_ENDIAN both defined.
#endif

#if defined(SM_WIN)
typedef int socklen_t;
#endif

// The following only works for C++
#ifdef __cplusplus

#ifndef ALIGNP
#define ALIGNP(p, t)                                             \
  (reinterpret_cast<uint8_t*>(((reinterpret_cast<uintptr_t>(p) + \
  ((t) - 1)) & ~((t) - 1))))
#endif

#define SM_IS_ALIGNED(p, a) (!((uintptr_t)(p) & ((a) - 1)))

// Use these to declare and define a static local variable that gets leaked so
// that its destructors are not called at exit.
#define SM_DEFINE_STATIC_LOCAL(type, name, arguments) \
  static type& name = *new type arguments

#endif  // __cplusplus

#endif  // SM_BASE_BASIC_TYPES_H_
