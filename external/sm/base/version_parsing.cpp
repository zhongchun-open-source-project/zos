// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/version_parsing.h"

#include <stdlib.h>

namespace sm {

bool ParseVersionString(const std::string& version_str,
                        int num_expected_segments,
                        int version[]) {
  size_t pos = 0;
  for (int i = 0;;) {
    size_t dot_pos = version_str.find('.', pos);
    size_t n;
    if (dot_pos == std::string::npos) {
      // npos here is a special value meaning "to the end of the string"
      n = std::string::npos;
    } else {
      n = dot_pos - pos;
    }

    version[i] = atoi(version_str.substr(pos, n).c_str());

    if (++i >= num_expected_segments) break;

    if (dot_pos == std::string::npos) {
      // Previous segment was not terminated by a dot, but there's supposed to
      // be more segments, so that's an error.
      return false;
    }
    pos = dot_pos + 1;
  }
  return true;
}

int CompareVersions(const int version1[],
                    const int version2[],
                    int num_segments) {
  for (int i = 0; i < num_segments; ++i) {
    int diff = version1[i] - version2[i];
    if (diff != 0) {
      return diff;
    }
  }
  return 0;
}

}  // namespace sm
