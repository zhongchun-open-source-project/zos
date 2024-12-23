# Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

file(GLOB_RECURSE sm_base_srcs
    "${sm_source_dir}/sm/base/*.h"
    "${sm_source_dir}/sm/base/*.c"
    "${sm_source_dir}/sm/base/*.cpp"
)

file(GLOB_RECURSE sm_base_platform_windows_srcs
    "${sm_source_dir}/sm/base/*.h"
    "${sm_source_dir}/sm/base/*.c"
    "${sm_source_dir}/sm/base/*.cpp"
)

file(GLOB_RECURSE sm_base_platform_linux_srcs
    "${sm_source_dir}/sm/base/*.h"
    "${sm_source_dir}/sm/base/*.h"
    "${sm_source_dir}/sm/base/*.cpp"
)

file(GLOB_RECURSE sm_base_platform_mac_srcs
    "${sm_source_dir}/sm/base/*.h"
    "${sm_source_dir}/sm/base/*.cpp"
)

file(GLOB_RECURSE sm_base_platform_android_srcs
    "${sm_source_dir}/sm/base/*.h"
    "${sm_source_dir}/sm/base/*.cpp"
)

file(GLOB_RECURSE sm_base_platform_posix_srcs
    "${sm_source_dir}/sm/base/*.h"
    "${sm_source_dir}/sm/base/*.cpp"
)

file(GLOB_RECURSE sm_base_test_srcs
    "${sm_source_dir}/sm/base/*.h"
    "${sm_source_dir}/sm/base/*.cpp"
)


list(REMOVE_ITEM sm_base_srcs ${sm_base_platform_windows_srcs})
list(REMOVE_ITEM sm_base_srcs ${sm_base_platform_linux_srcs})
list(REMOVE_ITEM sm_base_srcs ${sm_base_platform_mac_srcs})
list(REMOVE_ITEM sm_base_srcs ${sm_base_platform_android_srcs})
list(REMOVE_ITEM sm_base_srcs ${sm_base_platform_posix_srcs})
list(REMOVE_ITEM sm_base_srcs ${sm_base_platform_test_srcs})

if(WIN32)
list(APPEND sm_base_srcs ${sm_base_platform_windows_srcs})
endif()

include_directories("${sm_source_dir}")
add_library(sm_base OBJECT ${sm_base_srcs})
#add_dependencies(sm_base)