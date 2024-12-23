# Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

include (ExternalProject)

set(gemmlowp_URL https://github.com/google/gemmlowp/archive/38ebac7b059e84692f53e5938f97a9943c120d98.zip)
set(gemmlowp_HASH SHA256=b87faa7294dfcc5d678f22a59d2c01ca94ea1e2a3b488c38a95a67889ed0a658)
set(gemmlowp_BUILD ${CMAKE_CURRENT_BINARY_DIR}/gemmlowp/src/gemmlowp)
set(gemmlowp_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/gemmlowp/src/gemmlowp)

ExternalProject_Add(gemmlowp
    PREFIX gemmlowp
    URL ${gemmlowp_URL}
    URL_HASH ${gemmlowp_HASH}
    DOWNLOAD_DIR "${DOWNLOAD_LOCATION}"
    BUILD_IN_SOURCE 1
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/patches/gemmlowp/CMakeLists.txt ${gemmlowp_BUILD}
    INSTALL_COMMAND "")
