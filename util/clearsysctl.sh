#!/bin/sh

PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

for key in `sysctl keyremap4macbook 2>&1 | grep -oE '^keyremap4macbook.(remap|option).[^:]+'`; do
  sudo sysctl -w $key=0
done
