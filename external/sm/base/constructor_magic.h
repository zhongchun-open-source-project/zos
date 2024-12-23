// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_CONSTRUCTOR_MAGIC_H_
#define SM_BASE_CONSTRUCTOR_MAGIC_H_

// Put this in the declarations for a class to be unassignable.
#define SM_DISALLOW_ASSIGN(TypeName) \
  void operator=(const TypeName&) = delete

// A macro to disallow the copy constructor and operator= functions. This should
// be used in the declarations for a class.
#define SM_DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;          \
  SM_DISALLOW_ASSIGN(TypeName)

// A macro to disallow all the implicit constructors, namely the default
// constructor, copy constructor and operator= functions.
//
// This should be used in the declarations for a class that wants to prevent
// anyone from instantiating it. This is especially useful for classes
// containing only static methods.
#define SM_DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName() = delete;                               \
  SM_DISALLOW_COPY_AND_ASSIGN(TypeName)

#endif  // SM_BASE_CONSTRUCTOR_MAGIC_H_
