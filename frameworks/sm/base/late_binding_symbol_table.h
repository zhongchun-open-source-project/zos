// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_LATE_BINDING_SYMBOL_TABLE_H_
#define SM_BASE_LATE_BINDING_SYMBOL_TABLE_H_

#include <string.h>

#include "sm/base/common.h"

namespace sm {

#if defined(SM_POSIX)
typedef void *DllHandle;
#else
#error Not implemented for this platform
#endif

// This is the base class for "symbol table" classes to simplify the dynamic
// loading of symbols from DLLs. Currently the implementation only supports
// Linux and OS X, and pure C symbols (or extern "C" symbols that wrap C++
// functions).  Sub-classes for specific DLLs are generated via the "supermacro"
// files latebindingsymboltable.h.def and latebindingsymboltable.cc.def. See
// talk/sound/pulseaudiosymboltable.(h|cc) for an example.
class LateBindingSymbolTable {
 public:
  struct TableInfo {
    const char *dll_name;
    int num_symbols;
    // Array of size num_symbols.
    const char *const *symbol_names;
  };

  LateBindingSymbolTable(const TableInfo *info, void **table);
  ~LateBindingSymbolTable();

  bool IsLoaded() const;
  // Loads the DLL and the symbol table. Returns true iff the DLL and symbol
  // table loaded successfully.
  bool Load();
  // Like load, but allows overriding the dll path for when the dll path is
  // dynamic.
  bool LoadFromPath(const char *dll_path);
  void Unload();

  // Gets the raw OS handle to the DLL. Be careful what you do with it.
  DllHandle GetDllHandle() const { return handle_; }

 private:
  void ClearSymbols();

  const TableInfo *info_;
  void **table_;
  DllHandle handle_;
  bool undefined_symbols_;

  SM_DISALLOW_COPY_AND_ASSIGN(LateBindingSymbolTable);
};

}  // namespace sm

#endif  // SM_BASE_LATE_BINDING_SYMBOL_TABLE_H_
