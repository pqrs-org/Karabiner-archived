#!/bin/sh
PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

kext="/Library/org.pqrs/KeyRemap4MacBook/KeyRemap4MacBook.kext"
[ -d $kext ] && kextunload "$kext"

# old version
kext="/Applications/KeyRemap4MacBook/KeyRemap4MacBook.kext"
[ -d $kext ] && kextunload "$kext"

exit 0
