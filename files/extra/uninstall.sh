#!/bin/sh

/Library/org.pqrs/KeyRemap4MacBook/extra/uninstall_core.sh

# remove receipts
rm -f /var/db/receipts/org.pqrs.driver.KeyRemap4MacBook.*

# kill processes
/usr/bin/killall KeyRemap4MacBook_server 2>/dev/null

exit 0
