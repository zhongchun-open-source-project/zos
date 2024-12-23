// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_VERSION_PARSING_H_
#define SM_BASE_VERSION_PARSING_H_

#include <string>

namespace sm {

// Parses a version string into an array. "num_expected_segments" must be the
// number of numerical segments that the version is expected to have (e.g.,
// "1.1.2.0" has 4). "version" must be an array of that length to hold the
// parsed numbers.
// Returns "true" iff successful.
bool ParseVersionString(const std::string& version_str,
                        int num_expected_segments,
                        int version[]);

// Computes the lexicographical order of two versions. The return value
// indicates the order in the standard way (e.g., see strcmp()).
int CompareVersions(const int version1[],
                    const int version2[],
                    int num_segments);

}  // namespace sm

#endif  // SM_BASE_VERSION_PARSING_H_
