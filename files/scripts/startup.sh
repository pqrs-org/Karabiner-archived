#!/bin/sh
PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

basedir="/Applications/KeyRemap4MacBook.app/Contents/Library/scripts"

argument="$1"
[ -z "$argument" ] && argument=start
case "$argument" in
    start)
        echo "Starting KeyRemap4MacBook"
        $basedir/load.sh
        ;;

    stop)
        echo "Stopping KeyRemap4MacBook"
        $basedir/unload.sh
        ;;

    *)
        echo "Usage: $0 {start|stop}"
        ;;
esac

exit 0
