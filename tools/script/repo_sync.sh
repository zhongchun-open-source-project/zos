#!/bin/bash

exec -c nohup sudo /home/zcs/bin/repo sync -c > ~/android/repo-sync.log 2>&1 &
