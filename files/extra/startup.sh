#!/bin/sh
PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

basedir='/Library/Application Support/org.pqrs/Karabiner'

# --------------------
argument="$1"
[ -z "$argument" ] && argument=start
case "$argument" in
    start)
        echo "Starting Karabiner"
        kextload "$basedir/Karabiner.signed.kext"
        ;;

    stop)
        echo "Stopping Karabiner"
        kextunload -b org.pqrs.driver.Karabiner
        ;;

    *)
        echo "Usage: $0 {start|stop}"
        ;;
esac

exit 0
