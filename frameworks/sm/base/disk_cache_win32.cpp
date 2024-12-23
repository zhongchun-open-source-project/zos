// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/win32.h"
#include <shellapi.h>
#include <shlobj.h>
#include <tchar.h>

#include <time.h>
#include <algorithm>

#include "sm/base/common.h"
#include "sm/base/disk_cache.h"
#include "sm/base/path_utils.h"
#include "sm/base/stream.h"
#include "sm/base/string_encode.h"
#include "sm/base/string_utils.h"

#include "webrtc/base/diskcache_win32.h"

namespace sm {

bool DiskCacheWin32::InitializeEntries() {
  // Note: We could store the cache information in a separate file, for faster
  // initialization.  Figuring it out empirically works, too.

  std::wstring path16 = ToUtf16(folder_);
  path16.append(1, '*');

  WIN32_FIND_DATA find_data;
  HANDLE find_handle = FindFirstFile(path16.c_str(), &find_data);
  if (find_handle != INVALID_HANDLE_VALUE) {
    do {
      size_t index;
      std::string id;
      if (!FilenameToId(ToUtf8(find_data.cFileName), &id, &index))
        continue;

      Entry* entry = GetOrCreateEntry(id, true);
      entry->size += find_data.nFileSizeLow;
      total_size_ += find_data.nFileSizeLow;
      entry->streams = std::max(entry->streams, index + 1);
      FileTimeToUnixTime(find_data.ftLastWriteTime, &entry->last_modified);

    } while (FindNextFile(find_handle, &find_data));

    FindClose(find_handle);
  }

  return true;
}

bool DiskCacheWin32::PurgeFiles() {
  std::wstring path16 = ToUtf16(folder_);
  path16.append(1, '*');
  path16.append(1, '\0');

  SHFILEOPSTRUCT file_op = { 0 };
  file_op.wFunc = FO_DELETE;
  file_op.pFrom = path16.c_str();
  file_op.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT
                   | FOF_NORECURSION | FOF_FILESONLY;
  if (0 != SHFileOperation(&file_op)) {
    LOG_F(LS_ERROR) << "Couldn't delete cache files";
    return false;
  }

  return true;
}

bool DiskCacheWin32::FileExists(const std::string& filename) const {
  DWORD result = ::GetFileAttributes(ToUtf16(filename).c_str());
  return (INVALID_FILE_ATTRIBUTES != result);
}

bool DiskCacheWin32::DeleteFile(const std::string& filename) const {
  return ::DeleteFile(ToUtf16(filename).c_str()) != 0;
}

}
