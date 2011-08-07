#!/bin/sh

if `file -b "$1" | grep -sq ^Mach-O`; then
    # This is Mach-O file.

    # except kext bundle
    if `file -b "$1" | grep -sq 'kext bundle'`; then
        exit 1
    fi

    exit 0
fi

exit 1
