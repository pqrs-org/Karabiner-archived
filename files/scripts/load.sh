#!/bin/sh

PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

uname=`uname -r`
osmajor="${uname%%.*}"

/sbin/kextload /Applications/KeyRemap4MacBook.app/Contents/Library/KeyRemap4MacBook.signed.kext

exit 0
