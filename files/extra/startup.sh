#!/bin/sh
PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

basedir='/Library/Application Support/org.pqrs/Karabiner'

# --------------------
argument="$1"
[ -z "$argument" ] && argument=start
case "$argument" in
    start)
        if [ -f /etc/karabiner_kextload_delay ]; then
            delay=`ruby -e 'print $stdin.read.to_i' < /etc/karabiner_kextload_delay`
            sleep $delay
        fi

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
