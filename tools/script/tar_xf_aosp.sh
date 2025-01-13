#!/bin/bash

exec -c nohup tar -xf aosp-latest.tar > ~/aosp/tar-aosp.log 2>&1 &
