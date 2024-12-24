# Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

include (ExternalProject)

set(double_conversion_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/double_conversion/src/double_conversion)
set(double_conversion_URL https://github.com/google/double-conversion.git)
set(double_conversion_TAG 3992066a95b823efc8ccc1baf82a1cfc73f6e9b8)
set(double_conversion_BUILD ${double_conversion_INCLUDE_DIR})
set(double_conversion_LIBRARIES ${double_conversion_BUILD}/double-conversion/libdouble-conversion.so)
set(double_conversion_INCLUDES ${double_conversion_BUILD})

if(WIN32)
  set(double_conversion_STATIC_LIBRARIES ${double_conversion_BUILD}/$(Configuration)/double-conversion.lib)
else()
  set(double_conversion_STATIC_LIBRARIES ${double_conversion_BUILD}/libdouble-conversion.a)
endif()

set(double_conversion_HEADERS
    "${double_conversion_INCLUDE_DIR}/double-conversion/bignum-dtoa.h"
    "${double_conversion_INCLUDE_DIR}/double-conversion/cached-powers.h"
    "${double_conversion_INCLUDE_DIR}/double-conversion/double-conversion.h"
    "${double_conversion_INCLUDE_DIR}/double-conversion/fixed-dtoa.h"
    "${double_conversion_INCLUDE_DIR}/double-conversion/strtod.h"
    "${double_conversion_INCLUDE_DIR}/double-conversion/bignum.h"
    "${double_conversion_INCLUDE_DIR}/double-conversion/diy-fp.h"
    "${double_conversion_INCLUDE_DIR}/double-conversion/fast-dtoa.h"
    "${double_conversion_INCLUDE_DIR}/double-conversion/ieee.h"
    "${double_conversion_INCLUDE_DIR}/double-conversion/utils.h"
)

ExternalProject_Add(double_conversion
    PREFIX double_conversion
    GIT_REPOSITORY ${double_conversion_URL}
    GIT_TAG ${double_conversion_TAG}
    DOWNLOAD_DIR "${DOWNLOAD_LOCATION}"
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND ""
    CMAKE_CACHE_ARGS
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF
        -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=ON
)
