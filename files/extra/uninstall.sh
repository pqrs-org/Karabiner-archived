#!/bin/sh

sh '/Library/Application Support/org.pqrs/Karabiner/uninstall_core.sh'

# remove receipts
rm -f /var/db/receipts/org.pqrs.driver.Karabiner.*
rm -f /var/db/receipts/org.pqrs.driver.KeyRemap4MacBook.*

exit 0
