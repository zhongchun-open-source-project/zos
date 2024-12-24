// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_REFERENCE_COUNTED_SINGLETON_FACTORY_H_
#define SM_BASE_REFERENCE_COUNTED_SINGLETON_FACTORY_H_

#include "sm/base/common.h"
#include "sm/base/critical_section.h"
#include "sm/base/logging.h"
#include "sm/base/scoped_ptr.h"

namespace sm {

template <typename Interface> class rcsf_ptr;

// A ReferenceCountedSingletonFactory is an object which owns another object,
// and doles out the owned object to consumers in a reference-counted manner.
// Thus, the factory owns at most one object of the desired kind, and
// hands consumers a special pointer to it, through which they can access it.
// When the consumers delete the pointer, the reference count goes down,
// and if the reference count hits zero, the factory can throw the object
// away.  If a consumer requests the pointer and the factory has none,
// it can create one on the fly and pass it back.
template <typename Interface>
class ReferenceCountedSingletonFactory {
  friend class rcsf_ptr<Interface>;
 public:
  ReferenceCountedSingletonFactory() : ref_count_(0) {}

  virtual ~ReferenceCountedSingletonFactory() {
    ASSERT(ref_count_ == 0);
  }

 protected:
  // Must be implemented in a sub-class. The sub-class may choose whether or not
  // to cache the instance across lifetimes by either reset()'ing or not
  // reset()'ing the scoped_ptr in CleanupInstance().
  virtual bool SetupInstance() = 0;
  virtual void CleanupInstance() = 0;

  scoped_ptr<Interface> instance_;

 private:
  Interface* GetInstance() {
    CritScope cs(&crit_);
    if (ref_count_ == 0) {
      if (!SetupInstance()) {
        LOG(LS_VERBOSE) << "Failed to setup instance";
        return NULL;
      }
      ASSERT(instance_.get() != NULL);
    }
    ++ref_count_;

    LOG(LS_VERBOSE) << "Number of references: " << ref_count_;
    return instance_.get();
  }

  void ReleaseInstance() {
    CritScope cs(&crit_);
    ASSERT(ref_count_ > 0);
    ASSERT(instance_.get() != NULL);
    --ref_count_;
    LOG(LS_VERBOSE) << "Number of references: " << ref_count_;
    if (ref_count_ == 0) {
      CleanupInstance();
    }
  }

  CriticalSection crit_;
  int ref_count_;

  SM_DISALLOW_COPY_AND_ASSIGN(ReferenceCountedSingletonFactory);
};

template <typename Interface>
class rcsf_ptr {
 public:
  // Create a pointer that uses the factory to get the instance.
  // This is lazy - it won't generate the instance until it is requested.
  explicit rcsf_ptr(ReferenceCountedSingletonFactory<Interface>* factory)
      : instance_(NULL),
        factory_(factory) {
  }

  ~rcsf_ptr() {
    release();
  }

  Interface& operator*() {
    EnsureAcquired();
    return *instance_;
  }

  Interface* operator->() {
    EnsureAcquired();
    return instance_;
  }

  // Gets the pointer, creating the singleton if necessary. May return NULL if
  // creation failed.
  Interface* get() {
    Acquire();
    return instance_;
  }

  // Set instance to NULL and tell the factory we aren't using the instance
  // anymore.
  void release() {
    if (instance_) {
      instance_ = NULL;
      factory_->ReleaseInstance();
    }
  }

  // Lets us know whether instance is valid or not right now.
  // Even though attempts to use the instance will automatically create it, it
  // is advisable to check this because creation can fail.
  bool valid() const {
    return instance_ != NULL;
  }

  // Returns the factory that this pointer is using.
  ReferenceCountedSingletonFactory<Interface>* factory() const {
    return factory_;
  }

 private:
  void EnsureAcquired() {
    Acquire();
    ASSERT(instance_ != NULL);
  }

  void Acquire() {
    // Since we're getting a singleton back, acquire is a noop if instance is
    // already populated.
    if (!instance_) {
      instance_ = factory_->GetInstance();
    }
  }

  Interface* instance_;
  ReferenceCountedSingletonFactory<Interface>* factory_;

  SM_DISALLOW_IMPLICIT_CONSTRUCTORS(rcsf_ptr);
};

};  // namespace sm

#endif  // SM_BASE_REFERENCE_COUNTED_SINGLETON_FACTORY_H_
