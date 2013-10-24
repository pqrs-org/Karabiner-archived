#!/bin/bash

CODESIGN_IDENTITY="Developer ID Application: Fumihiko Takayama (G43BCU2T37)"

# ------------------------------------------------------------
PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

if [ ! -e "$1" ]; then
    echo "[ERROR] Invalid argument: '$1'"
    exit 1
fi

# ------------------------------------------------------------
# sign
for f in \
    `find "$1" -name '*.app' | sort -r` \
    ; do

    echo -ne '\033[33;40m'
    echo "code sign $f"
    echo -ne '\033[0m'

    echo -ne '\033[31;40m'
    codesign \
        --force \
        --deep \
        --sign "$CODESIGN_IDENTITY" \
        "$f"
    echo -ne '\033[0m'
done

# verify
for f in \
    `find "$1" -name '*.app'` \
    ; do
    echo -ne '\033[31;40m'
    codesign --verify --deep "$f"
    echo -ne '\033[0m'
done
