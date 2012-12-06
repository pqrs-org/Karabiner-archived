#!/bin/sh

/Applications/KeyRemap4MacBook.app/Contents/Library/extra/uninstall_core.sh

# remove receipts
rm -f /var/db/receipts/org.pqrs.driver.KeyRemap4MacBook.*

# kill processes
/usr/bin/killall KeyRemap4MacBook 2>/dev/null

exit 0
