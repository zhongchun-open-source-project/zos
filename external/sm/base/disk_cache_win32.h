// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_DISK_CACHE_WIN32_H_
#define SM_BASE_DISK_CACHE_WIN32_H_

#include "sm/base/disk_cache.h"

namespace sm {

class DiskCacheWin32 : public DiskCache {
 protected:
  virtual bool InitializeEntries();
  virtual bool PurgeFiles();

  virtual bool FileExists(const std::string& filename) const;
  virtual bool DeleteFile(const std::string& filename) const;
};

}

#endif  // SM_BASE_DISK_CACHE_WIN32_H_
