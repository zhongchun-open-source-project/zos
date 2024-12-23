# Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

# Minimum CMake required
cmake_minimum_required(VERSION 3.28)

# Project
project(sm VERSION 0.1.0)

# Set C++11 as standard for the whole project
set(CMAKE_CXX_STANDARD 11)

# Actual source is the ../.. directory
get_filename_component(sm_sm_source_dir ${sm_SOURCE_DIR} PATH)
get_filename_component(sm_source_dir ${sm_sm_source_dir} PATH)

# [CLEANUP] Not sure if this is needed (copied from Protobuf)
# CMake policies
cmake_policy(SET CMP0022 NEW)

# Options
option(sm_VERBOSE "Enable for verbose output" OFF)
option(sm_BUILD_CPP_TESTS "Build cpp unit tests " OFF)
option(sm_BUILD_SHARED_LIB "Build sm as a shared library" OFF)

if(WIN32)
# BoringSSL is disabled for windows as it currently doesn't build with
# MSBuild. (Ninja is required.)
option(sm_ENABLE_SSL_SUPPORT "Enable boringssl support" OFF)
else()
# BoringSSL is enabled for gRPC.
option(sm_ENABLE_SSL_SUPPORT "Enable boringssl support" ON)
endif()

if (NOT WIN32)
  # Threads: defines CMAKE_THREAD_LIBS_INIT and adds -pthread compile option
  # for targets that link ${CMAKE_THREAD_LIBS_INIT}.
  find_package (Threads REQUIRED)

  option(systemlib_ALL "Turn on every possible systemlib_* options" OFF)
  if (systemlib_ALL)
    set (systemlib_ZLIB ON)
    set (systemlib_ABSEIL_CPP ON)
  endif (systemlib_ALL)
endif()

if (WIN32)
  set(BOOL_WIN32 ON)
else (WIN32)
  set(BOOL_WIN32 OFF)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif (WIN32)

# [CLEANUP] Remove when done
# For debugging
function(SHOW_VARIABLES)
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endfunction()

# External dependencies
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/external ${PROJECT_SOURCE_DIR}/modules)

# Location where external projects will be downloaded
set (DOWNLOAD_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/downloads"
     CACHE PATH "Location where external projects will be downloaded.")
mark_as_advanced(DOWNLOAD_LOCATION)

if(WIN32)
  add_definitions(-DPLATFORM_WINDOWS)
endif()

# External dependencies
include(zlib)
include(png)
include(jpeg)
include(jsoncpp)
include(protobuf)
if (sm_BUILD_CPP_TESTS)
  include(googletest)
endif()

add_definitions(${ADD_CFLAGS})
link_directories(${ADD_LINK_DIRECTORY})

set(sm_EXTERNAL_LIBRARIES
    ${sm_EXTERNAL_LIBRARIES}
    ${gif_STATIC_LIBRARIES}
    ${png_STATIC_LIBRARIES}
    ${jpeg_STATIC_LIBRARIES}
    ${jsoncpp_STATIC_LIBRARIES}
    ${protobuf_STATIC_LIBRARIES}
)

include(sm_base.cmake)

if(sm_BUILD_SHARED_LIB)
  include(sm_shared_lib.cmake)
endif()

if(sm_BUILD_CPP_TESTS)
  include(sm_tests.cmake)
endif()