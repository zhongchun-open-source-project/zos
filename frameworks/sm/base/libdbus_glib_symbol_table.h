// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_LIBDBUS_GLIB_SYMBOL_TABLE_H_
#define SM_BASE_LIBDBUS_GLIB_SYMBOL_TABLE_H_

#ifdef HAVE_DBUS_GLIB

#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-lowlevel.h>

#include "sm/base/late_binding_symbol_table.h"

namespace sm {

#define LIBDBUS_GLIB_CLASS_NAME LibDBusGlibSymbolTable
// The libdbus-glib symbols we need, as an X-Macro list.
// This list must contain precisely every libdbus-glib function that is used in
// dbus.cc.
#define LIBDBUS_GLIB_SYMBOLS_LIST \
  X(dbus_bus_add_match) \
  X(dbus_connection_add_filter) \
  X(dbus_connection_close) \
  X(dbus_connection_remove_filter) \
  X(dbus_connection_set_exit_on_disconnect) \
  X(dbus_g_bus_get) \
  X(dbus_g_bus_get_private) \
  X(dbus_g_connection_get_connection) \
  X(dbus_g_connection_unref) \
  X(dbus_g_thread_init) \
  X(dbus_message_get_interface) \
  X(dbus_message_get_member) \
  X(dbus_message_get_path) \
  X(dbus_message_get_type) \
  X(dbus_message_iter_get_arg_type) \
  X(dbus_message_iter_get_basic) \
  X(dbus_message_iter_init) \
  X(dbus_message_ref) \
  X(dbus_message_unref)

#define LATE_BINDING_SYMBOL_TABLE_CLASS_NAME LIBDBUS_GLIB_CLASS_NAME
#define LATE_BINDING_SYMBOL_TABLE_SYMBOLS_LIST LIBDBUS_GLIB_SYMBOLS_LIST
#include "sm/base/late_binding_symbol_table.h.def"

}  // namespace sm

#endif  // HAVE_DBUS_GLIB

#endif  // SM_BASE_LIBDBUS_GLIB_SYMBOL_TABLE_H_
