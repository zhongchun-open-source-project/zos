// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_KEEP_REF_UNTIL_DONE_H_
#define SM_BASE_KEEP_REF_UNTIL_DONE_H_

#include "sm/base/bind.h"
#include "sm/base/callback.h"
#include "sm/base/refcount.h"
#include "sm/base/scoped_ref_ptr.h"

namespace sm {

namespace impl {
template <class T>
static inline void DoNothing(const scoped_refptr<T>& object) {}
}  // namespace impl

// KeepRefUntilDone keeps a reference to |object| until the returned
// callback goes out of scope. If the returned callback is copied, the
// reference will be released when the last callback goes out of scope.
template <class ObjectT>
static inline Callback0<void> KeepRefUntilDone(ObjectT* object) {
  return Bind(&impl::DoNothing<ObjectT>, scoped_refptr<ObjectT>(object));
}

template <class ObjectT>
static inline Callback0<void> KeepRefUntilDone(
    const scoped_refptr<ObjectT>& object) {
  return Bind(&impl::DoNothing<ObjectT>, object);
}

}  // namespace sm


#endif  // SM_BASE_KEEP_REF_UNTIL_DONE_H_
