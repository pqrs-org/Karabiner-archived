#!/bin/sh
PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

uid=`id -u`

# remove past directories.
rm -rf /tmp/KeyRemap4MacBook_server.$uid.*

tmpdir=`mktemp -d /tmp/KeyRemap4MacBook_server.$uid.XXXXXXXX`

# set socket_path
/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_set socket_path "$tmpdir/KeyRemap4MacBook_server"

# kick server
/Library/org.pqrs/KeyRemap4MacBook/app/KeyRemap4MacBook_server.app/Contents/MacOS/KeyRemap4MacBook_server "$tmpdir"

exit 0
