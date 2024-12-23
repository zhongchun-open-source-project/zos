// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

// Stores a collection of pointers that are deleted when the container is
// destructed.

#ifndef SM_BASE_SCOPEDPTR_COLLECTION_H_
#define SM_BASE_SCOPEDPTR_COLLECTION_H_

#include <algorithm>
#include <vector>

#include "sm/base/basic_types.h"
#include "sm/base/constructor_magic.h"

namespace sm {

template<class T>
class ScopedPtrCollection {
 public:
  typedef std::vector<T*> VectorT;

  ScopedPtrCollection() { }
  ~ScopedPtrCollection() {
    for (typename VectorT::iterator it = collection_.begin();
         it != collection_.end(); ++it) {
      delete *it;
    }
  }

  const VectorT& collection() const { return collection_; }
  void Reserve(size_t size) {
    collection_.reserve(size);
  }
  void PushBack(T* t) {
    collection_.push_back(t);
  }

  // Remove |t| from the collection without deleting it.
  void Remove(T* t) {
    collection_.erase(std::remove(collection_.begin(), collection_.end(), t),
                      collection_.end());
  }

 private:
  VectorT collection_;

  SM_DISALLOW_COPY_AND_ASSIGN(ScopedPtrCollection);
};

}  // namespace sm

#endif  // SM_BASE_SCOPEDPTR_COLLECTION_H_
