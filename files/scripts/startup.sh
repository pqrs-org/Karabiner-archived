#!/bin/sh
PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

basedir="/Library/org.pqrs/KeyRemap4MacBook"

argument="$1"
[ -z "$argument" ] && argument=start
case "$argument" in
    start)
        echo "Starting KeyRemap4MacBook"
        sleep 10 # wait for avoid kext loading collision.
        $basedir/scripts/load.sh
        ;;

    quickstart)
        echo "Starting KeyRemap4MacBook"
        $basedir/scripts/load.sh
        ;;

    stop)
        echo "Stopping KeyRemap4MacBook"
        $basedir/scripts/unload.sh
        ;;

    *)
        echo "Usage: $0 {start|stop}"
        ;;
esac

exit 0
