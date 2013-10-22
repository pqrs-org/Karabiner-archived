#!/bin/sh

PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

uname=`uname -r`
osmajor="${uname%%.*}"

if [ $osmajor -ge 13 ]; then
    # We should load signed kext since OS X 10.9.
    /sbin/kextload /Applications/KeyRemap4MacBook.app/Contents/Library/KeyRemap4MacBook.signed.kext
else
    # We need to load not signed kext OS X 10.8 or prior.
    /sbin/kextload /Applications/KeyRemap4MacBook.app/Contents/Library/KeyRemap4MacBook.kext
fi

exit 0
