#! /bin/sh

until [ "x`sysctl -n keyremap4macbook.initialized 2>/dev/null`" = "x1" ]; do
    echo "Waiting reconfigure..."
    sleep 0.5
done

echo
exit 0
