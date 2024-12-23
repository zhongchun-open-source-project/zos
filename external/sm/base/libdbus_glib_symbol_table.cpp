// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifdef HAVE_DBUS_GLIB

#include "sm/base/libdbus_glib_symbol_table.h"

namespace sm {

#define LATE_BINDING_SYMBOL_TABLE_CLASS_NAME LIBDBUS_GLIB_CLASS_NAME
#define LATE_BINDING_SYMBOL_TABLE_SYMBOLS_LIST LIBDBUS_GLIB_SYMBOLS_LIST
#define LATE_BINDING_SYMBOL_TABLE_DLL_NAME "libdbus-glib-1.so.2"
#include "sm/base/late_binding_symbol_table.cc.def"

}  // namespace sm

#endif  // HAVE_DBUS_GLIB
