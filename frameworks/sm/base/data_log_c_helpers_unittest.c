// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/data_log_c_helpers_unittest.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "sm/base/data_log_c.h"

enum { kTestArrayLen = 4 };
static const char kTableName[] = "c_wrapper_table";
static const char kColumnName1[] = "Scalar";
static const char kColumnName2[] = "Vector";

int SmDataLogCHelper_TestCreateLog() {
  return SmDataLog_CreateLog();
}

int SmDataLogCHelper_TestReturnLog() {
  SmDataLog_ReturnLog();
  return 0;
}

int SmDataLogCHelper_TestCombine() {
  const int kOutLen = strlen(kTableName) + 4;  // Room for "_17" + '\0'
  char* combined_name = malloc(kOutLen * sizeof(char));
  char* out_ptr = SmDataLog_Combine(combined_name, kOutLen, kTableName, 17);
  int return_code = 0;
  if (!out_ptr) {
    return_code = -1;
  }
  if (strcmp(combined_name, "c_wrapper_table_17") != 0) {
    return_code = -2;
  }
  free(combined_name);
  return return_code;
}

int SmDataLogCHelper_TestAddTable() {
  return SmDataLog_AddTable(kTableName);
}

int SmDataLogCHelper_TestAddColumn() {
  if (SmDataLog_AddColumn(kTableName, kColumnName1, 1) != 0) {
    return -1;
  }
  if (SmDataLog_AddColumn(kTableName, kColumnName2, kTestArrayLen) != 0) {
    return -2;
  }
  return 0;
}

int SmDataLogCHelper_TestNextRow() {
  return SmDataLog_NextRow(kTableName);
}

int SmDataLogCHelper_TestInsertCell_int() {
  return SmDataLog_InsertCell_int(kTableName, kColumnName1, 17);
}

int SmDataLogCHelper_TestInsertArray_int() {
  int values[kTestArrayLen] = {1, 2, 3, 4};
  return SmDataLog_InsertArray_int(kTableName, kColumnName2, values,
                                       kTestArrayLen);
}

int SmDataLogCHelper_TestInsertCell_float() {
  return SmDataLog_InsertCell_float(kTableName, kColumnName1, 17.0f);
}

int SmDataLogCHelper_TestInsertArray_float() {
  float values[kTestArrayLen] = {1.0f, 2.0f, 3.0f, 4.0f};
  return SmDataLog_InsertArray_float(kTableName, kColumnName2, values,
                                         kTestArrayLen);
}

int SmDataLogCHelper_TestInsertCell_double() {
  return SmDataLog_InsertCell_int(kTableName, kColumnName1, 17.0);
}

int SmDataLogCHelper_TestInsertArray_double() {
  double values[kTestArrayLen] = {1.0, 2.0, 3.0, 4.0};
  return SmDataLog_InsertArray_double(kTableName, kColumnName2, values,
                                          kTestArrayLen);
}

int SmDataLogCHelper_TestInsertCell_int32() {
  return SmDataLog_InsertCell_int32(kTableName, kColumnName1, 17);
}

int SmDataLogCHelper_TestInsertArray_int32() {
  int32_t values[kTestArrayLen] = {1, 2, 3, 4};
  return SmDataLog_InsertArray_int32(kTableName, kColumnName2, values,
                                         kTestArrayLen);
}

int SmDataLogCHelper_TestInsertCell_uint32() {
  return SmDataLog_InsertCell_uint32(kTableName, kColumnName1, 17);
}

int SmDataLogCHelper_TestInsertArray_uint32() {
  uint32_t values[kTestArrayLen] = {1, 2, 3, 4};
  return SmDataLog_InsertArray_uint32(kTableName, kColumnName2, values,
                                          kTestArrayLen);
}

int SmDataLogCHelper_TestInsertCell_int64() {
  return SmDataLog_InsertCell_int64(kTableName, kColumnName1, 17);
}

int SmDataLogCHelper_TestInsertArray_int64() {
  int64_t values[kTestArrayLen] = {1, 2, 3, 4};
  return SmDataLog_InsertArray_int64(kTableName, kColumnName2, values,
                                         kTestArrayLen);
}
