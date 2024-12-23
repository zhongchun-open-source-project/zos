# Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

include (ExternalProject)

set(cub_URL https://mirror.bazel.build/github.com/NVlabs/cub/archive/1.8.0.zip)
set(cub_HASH SHA256=6bfa06ab52a650ae7ee6963143a0bbc667d6504822cbd9670369b598f18c58c3)
set(cub_BUILD ${CMAKE_CURRENT_BINARY_DIR}/cub/src/cub)
set(cub_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/cub/src/cub)
set(cub_ARCHIVE_DIR ${CMAKE_CURRENT_BINARY_DIR}/external/cub_archive)

ExternalProject_Add(cub
    PREFIX cub
    URL ${cub_URL}
    URL_HASH ${cub_HASH}
    DOWNLOAD_DIR "${DOWNLOAD_LOCATION}"
    BUILD_IN_SOURCE 1
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/patches/cub/CMakeLists.txt ${cub_BUILD}
    INSTALL_COMMAND  ${CMAKE_COMMAND} -E copy_directory  ${cub_INCLUDE_DIR}/cub ${cub_ARCHIVE_DIR}/cub)
