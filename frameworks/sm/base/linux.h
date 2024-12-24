// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_LINUX_H_
#define SM_BASE_LINUX_H_

#if defined(SM_LINUX)
#include <string>
#include <map>
#include <vector>

#include "sm/base/scoped_ptr.h"
#include "sm/base/stream.h"

namespace sm {

//////////////////////////////////////////////////////////////////////////////
// ConfigParser parses a FileStream of an ".ini."-type format into a map.
//////////////////////////////////////////////////////////////////////////////

// Sample Usage:
//   ConfigParser parser;
//   ConfigParser::MapVector key_val_pairs;
//   if (parser.Open(inifile) && parser.Parse(&key_val_pairs)) {
//     for (int section_num=0; i < key_val_pairs.size(); ++section_num) {
//       std::string val1 = key_val_pairs[section_num][key1];
//       std::string val2 = key_val_pairs[section_num][key2];
//       // Do something with valn;
//     }
//   }

class ConfigParser {
 public:
  typedef std::map<std::string, std::string> SimpleMap;
  typedef std::vector<SimpleMap> MapVector;

  ConfigParser();
  virtual ~ConfigParser();

  virtual bool Open(const std::string& filename);
  virtual void Attach(StreamInterface* stream);
  virtual bool Parse(MapVector* key_val_pairs);
  virtual bool ParseSection(SimpleMap* key_val_pair);
  virtual bool ParseLine(std::string* key, std::string* value);

 private:
  scoped_ptr<StreamInterface> instream_;
};

//////////////////////////////////////////////////////////////////////////////
// ProcCpuInfo reads CPU info from the /proc subsystem on any *NIX platform.
//////////////////////////////////////////////////////////////////////////////

// Sample Usage:
//   ProcCpuInfo proc_info;
//   int no_of_cpu;
//   if (proc_info.LoadFromSystem()) {
//      std::string out_str;
//      proc_info.GetNumCpus(&no_of_cpu);
//      proc_info.GetCpuStringValue(0, "vendor_id", &out_str);
//      }
//   }

class ProcCpuInfo {
 public:
  ProcCpuInfo();
  virtual ~ProcCpuInfo();

  // Reads the proc subsystem's cpu info into memory. If this fails, this
  // returns false; if it succeeds, it returns true.
  virtual bool LoadFromSystem();

  // Obtains the number of logical CPU threads and places the value num.
  virtual bool GetNumCpus(int* num);

  // Obtains the number of physical CPU cores and places the value num.
  virtual bool GetNumPhysicalCpus(int* num);

  // Obtains the CPU family id.
  virtual bool GetCpuFamily(int* id);

  // Obtains the number of sections in /proc/cpuinfo, which may be greater
  // than the number of CPUs (e.g. on ARM)
  virtual bool GetSectionCount(size_t* count);

  // Looks for the CPU proc item with the given name for the given section
  // number and places the string value in result.
  virtual bool GetSectionStringValue(size_t section_num, const std::string& key,
                                     std::string* result);

  // Looks for the CPU proc item with the given name for the given section
  // number and places the int value in result.
  virtual bool GetSectionIntValue(size_t section_num, const std::string& key,
                                  int* result);

 private:
  ConfigParser::MapVector sections_;
};

// Returns the output of "uname".
std::string ReadLinuxUname();

// Returns the content (int) of
// /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq
// Returns -1 on error.
int ReadCpuMaxFreq();

}  // namespace sm

#endif  // defined(SM_LINUX)
#endif  // SM_BASE_LINUX_H_
