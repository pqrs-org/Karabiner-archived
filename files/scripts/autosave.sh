#!/bin/sh
PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

basedir="/Applications/KeyRemap4MacBook"

[ "`sysctl -n keyremap4macbook.changed`" != "1" ] && exit 0

"$basedir/scripts/save.sh"
sysctl -w keyremap4macbook.changed=0
exit 0
