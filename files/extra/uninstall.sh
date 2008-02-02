#!/bin/sh
PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

# ----------------------------------------
# uninstall oldstyle-files
targetfile="/Library/StartupItems/KeyRemap4MacBook/KeyRemap4MacBook.kext/Contents/Info.plist"
if [ -f "$targetfile" ]; then
    if grep -q org.pqrs "$targetfile"; then
        /Library/StartupItems/KeyRemap4MacBook/KeyRemap4MacBook stop
        rm -rf "/Library/StartupItems/KeyRemap4MacBook/"
    fi
fi

# ----------------------------------------
# uninstall
basedir="/Applications/KeyRemap4MacBook"

"$basedir/scripts/unload.sh"
rm -f "/Library/LaunchDaemons/org.pqrs.KeyRemap4MacBook.load.plist"
rm -f "/Library/LaunchDaemons/org.pqrs.KeyRemap4MacBook.autosave.plist"
rm -rf "/Library/PreferencePanes/KeyRemap4MacBook.prefPane"
rm -rf "$basedir"

exit 0
