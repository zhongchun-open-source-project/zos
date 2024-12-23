// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

// This is a pure C wrapper of the DataLog class. The functions are directly
// mapped here except for InsertCell as C does not support templates.
// See data_log.h for a description of the functions.

#ifndef SRC_BASE_DATA_LOG_C_H_
#define SRC_BASE_DATA_LOG_C_H_

#include <stddef.h>  // size_t

#include "sm/typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

// All char* parameters in this file are expected to be null-terminated
// character sequences.
int SmDataLog_CreateLog();
void SmDataLog_ReturnLog();
char* SmDataLog_Combine(char* combined_name, size_t combined_len,
                            const char* table_name, int table_id);
int SmDataLog_AddTable(const char* table_name);
int SmDataLog_AddColumn(const char* table_name, const char* column_name,
                            int multi_value_length);

int SmDataLog_InsertCell_int(const char* table_name,
                                 const char* column_name,
                                 int value);
int SmDataLog_InsertArray_int(const char* table_name,
                                  const char* column_name,
                                  const int* values,
                                  int length);
int SmDataLog_InsertCell_float(const char* table_name,
                                   const char* column_name,
                                   float value);
int SmDataLog_InsertArray_float(const char* table_name,
                                    const char* column_name,
                                    const float* values,
                                    int length);
int SmDataLog_InsertCell_double(const char* table_name,
                                    const char* column_name,
                                    double value);
int SmDataLog_InsertArray_double(const char* table_name,
                                     const char* column_name,
                                     const double* values,
                                     int length);
int SmDataLog_InsertCell_int32(const char* table_name,
                                   const char* column_name,
                                   int32_t value);
int SmDataLog_InsertArray_int32(const char* table_name,
                                    const char* column_name,
                                    const int32_t* values,
                                    int length);
int SmDataLog_InsertCell_uint32(const char* table_name,
                                    const char* column_name,
                                    uint32_t value);
int SmDataLog_InsertArray_uint32(const char* table_name,
                                     const char* column_name,
                                     const uint32_t* values,
                                     int length);
int SmDataLog_InsertCell_int64(const char* table_name,
                                   const char* column_name,
                                   int64_t value);
int SmDataLog_InsertArray_int64(const char* table_name,
                                    const char* column_name,
                                    const int64_t* values,
                                    int length);

int SmDataLog_NextRow(const char* table_name);

#ifdef __cplusplus
}  // end of extern "C"
#endif

#endif  // SRC_BASE_DATA_LOG_C_H_  // NOLINT
