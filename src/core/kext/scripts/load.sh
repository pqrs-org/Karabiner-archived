#!/bin/sh

sudo cp -R build/Release/KeyRemap4MacBook.kext /tmp/
sudo kextload -t /tmp/KeyRemap4MacBook.kext

#--------------------
while true; do
    echo "Waiting reconfigure..."
    [ "x`sysctl -n keyremap4macbook.initialized`" = "x1" ] && exit 0
    sleep 1
done
