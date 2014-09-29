#!/bin/sh

clang_format="$HOME/Library/Application Support/Alcatraz/Plug-ins/ClangFormat/bin/clang-format"
format_options='-i -style=Google'

find ../../* \
    \( -name '*.[ch]pp' -o -name '*.[mh]' \) \
    -type f \
    ! -ipath '*/Pods/*' \
    ! -ipath '*/build/*' \
    \
    | xargs "$clang_format" $format_options
