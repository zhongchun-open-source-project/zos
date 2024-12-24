#!/usr/bin/env bash

# Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

(
cd "$(dirname "$0")"
mkdir -p _build

(
cd _build
rm -rf -- *
cmake ..
)
)
