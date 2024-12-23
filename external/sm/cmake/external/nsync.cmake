# Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

include (ExternalProject)

set(nsync_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/external/nsync/public)
set(nsync_URL https://github.com/google/nsync)
set(nsync_TAG 1.20.2)
set(nsync_BUILD ${CMAKE_CURRENT_BINARY_DIR}/nsync/src/nsync)
set(nsync_INSTALL ${CMAKE_CURRENT_BINARY_DIR}/nsync/install)

if(WIN32)
  set(nsync_HEADERS "${nsync_BUILD}/public/*.h")
  set(nsync_STATIC_LIBRARIES ${nsync_INSTALL}/lib/nsync_cpp.lib)
else()
  set(nsync_HEADERS "${nsync_BUILD}/public/*.h")
  set(nsync_STATIC_LIBRARIES ${nsync_INSTALL}/lib/libnsync_cpp.a)
endif()

ExternalProject_Add(nsync
    PREFIX nsync
    GIT_REPOSITORY ${nsync_URL}
    GIT_TAG ${nsync_TAG}
    DOWNLOAD_DIR "${DOWNLOAD_LOCATION}"
    BUILD_IN_SOURCE 1
    BUILD_BYPRODUCTS ${nsync_STATIC_LIBRARIES}
    INSTALL_DIR ${nsync_INSTALL}
    CMAKE_CACHE_ARGS
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF
        -DCMAKE_INSTALL_PREFIX:STRING=${nsync_INSTALL}
        -DCMAKE_INSTALL_LIBDIR:STRING=lib
    -DNSYNC_LANGUAGE:STRING=c++11)

set(nsync_HEADERS
    "${nsync_INSTALL}/include/nsync.h"
    "${nsync_INSTALL}/include/nsync_atomic.h"
    "${nsync_INSTALL}/include/nsync_counter.h"
    "${nsync_INSTALL}/include/nsync_cpp.h"
    "${nsync_INSTALL}/include/nsync_cv.h"
    "${nsync_INSTALL}/include/nsync_debug.h"
    "${nsync_INSTALL}/include/nsync_mu.h"
    "${nsync_INSTALL}/include/nsync_mu_wait.h"
    "${nsync_INSTALL}/include/nsync_note.h"
    "${nsync_INSTALL}/include/nsync_once.h"
    "${nsync_INSTALL}/include/nsync_time.h"
    "${nsync_INSTALL}/include/nsync_time_internal.h"
    "${nsync_INSTALL}/include/nsync_waiter.h"
)
                                                            
# put nsync includes in the directory where they are expected
add_custom_target(nsync_create_destination_dir
    COMMAND ${CMAKE_COMMAND} -E make_directory ${nsync_INCLUDE_DIR}
    DEPENDS nsync)

add_custom_target(nsync_copy_headers_to_destination
    DEPENDS nsync_create_destination_dir)

foreach(header_file ${nsync_HEADERS})
  add_custom_command(TARGET nsync_copy_headers_to_destination PRE_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${header_file} ${nsync_INCLUDE_DIR}/)
endforeach()


