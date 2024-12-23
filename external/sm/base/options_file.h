// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_OPTIONS_FILE_H_
#define SM_BASE_OPTIONS_FILE_H_

#include <map>
#include <string>

namespace sm {

// Implements storage of simple options in a text file on disk. This is
// cross-platform, but it is intended mostly for Linux where there is no
// first-class options storage system.
class OptionsFile {
 public:
  OptionsFile(const std::string &path);
  ~OptionsFile();

  // Loads the file from disk, overwriting the in-memory values.
  bool Load();
  // Saves the contents in memory, overwriting the on-disk values.
  bool Save();

  bool GetStringValue(const std::string& option, std::string* out_val) const;
  bool GetIntValue(const std::string& option, int* out_val) const;
  bool SetStringValue(const std::string& option, const std::string& val);
  bool SetIntValue(const std::string& option, int val);
  bool RemoveValue(const std::string& option);

 private:
  typedef std::map<std::string, std::string> OptionsMap;

  static bool IsLegalName(const std::string &name);
  static bool IsLegalValue(const std::string &value);

  std::string path_;
  OptionsMap options_;
};

}  // namespace sm

#endif  // SM_BASE_OPTIONS_FILE_H_
