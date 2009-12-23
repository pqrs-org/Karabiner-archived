#!/bin/sh
PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

uid=`id -u`

# remove past directories.
rm -rf /tmp/KeyRemap4MacBook.$uid.*

tmpdir=`mktemp -d /tmp/KeyRemap4MacBook.$uid.XXXXXXXX`

# kick server
exec /Library/org.pqrs/KeyRemap4MacBook/app/KeyRemap4MacBook.app/Contents/MacOS/KeyRemap4MacBook "$tmpdir"
