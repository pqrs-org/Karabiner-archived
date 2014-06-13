#! /bin/sh

until [ "x`sysctl -n karabiner.initialized 2>/dev/null`" = "x1" ]; do
    echo "Waiting reconfigure..."
    sleep 0.5
done

echo
exit 0
