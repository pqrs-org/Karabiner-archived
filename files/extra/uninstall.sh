#!/bin/sh
PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

# ----------------------------------------
# unload
startup="/Library/StartupItems/KeyRemap4MacBook/KeyRemap4MacBook"
[ -f "$startup" ] && "$startup" stop

startup="/Applications/KeyRemap4MacBook/scripts/startup.sh"
[ -f "$startup" ] && "$startup" stop

startup="/Library/org.pqrs/KeyRemap4MacBook/scripts/startup.sh"
[ -f "$startup" ] && "$startup" stop

for f in /Library/LaunchDaemons/org.pqrs.KeyRemap4MacBook.* /Library/LaunchAgents/org.pqrs.KeyRemap4MacBook.*; do
    [ -f $f ] && launchctl unload $f
done

# ----------------------------------------
# uninstall files
rm -rf /Library/StartupItems/KeyRemap4MacBook
rm -rf /Applications/KeyRemap4MacBook
rm -rf /Library/org.pqrs/KeyRemap4MacBook

rm -rf /Library/PreferencePanes/KeyRemap4MacBook.prefPane

rm -ff /Library/LaunchDaemons/org.pqrs.KeyRemap4MacBook.*
rm -ff /Library/LaunchAgents/org.pqrs.KeyRemap4MacBook.*

exit 0
