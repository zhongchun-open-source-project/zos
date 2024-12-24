// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_GUNIT_PROD_H_
#define SM_BASE_GUNIT_PROD_H_

#if defined(SM_ANDROID)
// Android doesn't use gtest at all, so anything that relies on gtest should
// check this define first.
#define NO_GTEST
#elif defined (GTEST_RELATIVE_PATH)
#include "gtest/gtest_prod.h"
#else
#include "testing/base/gunit_prod.h"
#endif

#endif  // SM_BASE_GUNIT_PROD_H_
