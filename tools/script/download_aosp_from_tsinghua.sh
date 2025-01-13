#!/bin/bash

exec -c nohup wget --quiet --user-agent="Mozilla" --no-check-certificate https://mirrors.tuna.tsinghua.edu.cn/aosp-monthly/aosp-latest.tar > ~/aosp/wget-aosp.log 2>&1 &