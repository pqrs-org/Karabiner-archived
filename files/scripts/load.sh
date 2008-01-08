#!/bin/sh
PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

basedir="/Applications/KeyRemap4MacBook"
kextfile="$basedir/KeyRemap4MacBook.kext"

kextload "$kextfile"
sleep 3 # wait for a while just after kextload.

sysctl="$basedir/scripts/sysctl.sh"
[ -f "$sysctl" ] && "$sysctl"

"$basedir/scripts/save.sh"

exit 0
