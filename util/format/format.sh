#!/bin/sh

find ../../* \
    \( -name '*.[ch]pp' -o -name '*.[mh]' \) \
    -type f \
    ! -ipath '*/Pods/*' \
    ! -ipath '*/build/*' \
    \
    | xargs clang-format -i
