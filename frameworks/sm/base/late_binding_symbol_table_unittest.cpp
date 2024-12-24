// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#if defined(SM_LINUX) && !defined(SM_ANDROID)
#include <math.h>
#endif

#include "sm/base/gunit.h"
#include "sm/base/late_binding_symbol_table.h"

namespace sm {

#if defined(SM_LINUX) && !defined(SM_ANDROID)

#define LIBM_SYMBOLS_CLASS_NAME LibmTestSymbolTable
#define LIBM_SYMBOLS_LIST \
  X(acosf) \
  X(sinf) \
  X(tanf)


#define LATE_BINDING_SYMBOL_TABLE_CLASS_NAME LIBM_SYMBOLS_CLASS_NAME
#define LATE_BINDING_SYMBOL_TABLE_SYMBOLS_LIST LIBM_SYMBOLS_LIST
#include "sm/base/late_binding_symbol_table.h.def"

#define LATE_BINDING_SYMBOL_TABLE_CLASS_NAME LIBM_SYMBOLS_CLASS_NAME
#define LATE_BINDING_SYMBOL_TABLE_SYMBOLS_LIST LIBM_SYMBOLS_LIST
#define LATE_BINDING_SYMBOL_TABLE_DLL_NAME "libm.so.6"
#include "sm/base/late_binding_symbol_table.cc.def"

TEST(LateBindingSymbolTable, libm) {
  LibmTestSymbolTable table;
  EXPECT_FALSE(table.IsLoaded());
  ASSERT_TRUE(table.Load());
  EXPECT_TRUE(table.IsLoaded());
  EXPECT_EQ(table.acosf()(0.5f), acosf(0.5f));
  EXPECT_EQ(table.sinf()(0.5f), sinf(0.5f));
  EXPECT_EQ(table.tanf()(0.5f), tanf(0.5f));
  // It would be nice to check that the addresses are the same, but the nature
  // of dynamic linking and relocation makes them actually be different.
  table.Unload();
  EXPECT_FALSE(table.IsLoaded());
}

#else
#error Not implemented
#endif

}  // namespace sm
