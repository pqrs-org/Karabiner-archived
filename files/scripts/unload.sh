#!/bin/sh
PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

basedir="/Applications/KeyRemap4MacBook"
kextfile="$basedir/KeyRemap4MacBook.kext"

"$basedir/scripts/save.sh"
kextunload "$kextfile"

exit 0
