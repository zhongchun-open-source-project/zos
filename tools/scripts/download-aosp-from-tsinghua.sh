#!/bin/bash

exec -c nohup wget --quiet https://mirrors.tuna.tsinghua.edu.cn/aosp-monthly/aosp-20241201.tar > ~/android/wget.log 2>&1 &

