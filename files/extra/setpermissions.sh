#!/bin/sh

PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

if [ ! -e "$1" ]; then
    echo "[ERROR] Invalid argument: '$1'"
    exit 1
fi

# ------------------------------------------------------------
test_mach_o() {
    if `file -b "$1" | grep -sq ^Mach-O`; then
        # This is Mach-O file.

        # except kext bundle
        if `file -b "$1" | grep -sq 'kext bundle'`; then
            exit 1
        fi

        exit 0
    fi
    exit 1
}

# ------------------------------------------------------------
find "$1" -type d -print0 | xargs -0 chmod 755
find "$1" -type f -print0 | xargs -0 chmod 644
find "$1" -name '*.sh' -print0 | xargs -0 chmod 755
find "$1" -type f -print0 | while read -d $'\0' file; do
    if `test_mach_o "$file"`; then
        chmod 755 "$file"
    fi
done
chown -R root:wheel "$1"
