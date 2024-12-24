// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_COMPILE_ASSERT_H_
#define SM_BASE_COMPILE_ASSERT_H_

#ifdef __cplusplus
#error "Only use this for C files. For C++, use static_assert."
#endif

// Use this macro to verify at compile time that certain restrictions are met.
// The argument is the boolean expression to evaluate.
// Example:
//   COMPILE_ASSERT(sizeof(foo) < 128);
#define COMPILE_ASSERT(expression) switch (0) {case 0: case expression:;}

#endif  // SM_BASE_COMPILE_ASSERT_H_
