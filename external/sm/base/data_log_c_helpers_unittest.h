// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_DATA_LOG_C_HELPERS_UNITTEST_H_
#define SM_BASE_DATA_LOG_C_HELPERS_UNITTEST_H_

#ifdef __cplusplus
extern "C" {
#endif

int SmDataLogCHelper_TestCreateLog();

int SmDataLogCHelper_TestReturnLog();

int SmDataLogCHelper_TestCombine();

int SmDataLogCHelper_TestAddTable();

int SmDataLogCHelper_TestAddColumn();

int SmDataLogCHelper_TestNextRow();

int SmDataLogCHelper_TestInsertCell_int();

int SmDataLogCHelper_TestInsertArray_int();

int SmDataLogCHelper_TestInsertCell_float();

int SmDataLogCHelper_TestInsertArray_float();

int SmDataLogCHelper_TestInsertCell_double();

int SmDataLogCHelper_TestInsertArray_double();

int SmDataLogCHelper_TestInsertCell_int32();

int SmDataLogCHelper_TestInsertArray_int32();

int SmDataLogCHelper_TestInsertCell_uint32();

int SmDataLogCHelper_TestInsertArray_uint32();

int SmDataLogCHelper_TestInsertCell_int64();

int SmDataLogCHelper_TestInsertArray_int64();

#ifdef __cplusplus
}  // end of extern "C"
#endif

#endif  // SM_BASE_DATA_LOG_C_HELPERS_UNITTEST_H_
