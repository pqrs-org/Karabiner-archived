#! /bin/sh

until [ "x`sysctl -n keyremap4macbook.initialized`" = "x1" ]; do
    echo "Waiting reconfigure..."
    sleep 1
done

/bin/echo -n "." && sleep 1
/bin/echo -n "." && sleep 1
/bin/echo -n "." && sleep 1

echo
exit 0
