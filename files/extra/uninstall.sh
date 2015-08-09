#!/bin/sh

sh '/Library/Application Support/org.pqrs/Karabiner/uninstall_core.sh'

# remove receipts
rm -f /var/db/receipts/org.pqrs.driver.Karabiner.*
rm -f /var/db/receipts/org.pqrs.driver.KeyRemap4MacBook.*

# kill processes
/usr/bin/killall Karabiner 2>/dev/null

exit 0
