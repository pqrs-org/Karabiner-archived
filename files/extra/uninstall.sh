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
# version 2.0 - 3.x
basedir="/Applications/KeyRemap4MacBook"
if [ -d $basedir ]; then
    "$basedir/scripts/unload.sh"
    rm -rf "$basedir"
fi

# version 3.x - current
basedir="/Library/org.pqrs/KeyRemap4MacBook"
if [ -d $basedir ]; then
    "$basedir/scripts/unload.sh"
    rm -rf "$basedir"
fi

launchctl unload /Library/LaunchDaemons/org.pqrs.KeyRemap4MacBook.server.plist

rm -f "/Library/LaunchDaemons/org.pqrs.KeyRemap4MacBook.load.plist"
rm -f "/Library/LaunchDaemons/org.pqrs.KeyRemap4MacBook.autosave.plist"
rm -f "/Library/LaunchDaemons/org.pqrs.KeyRemap4MacBook.server.plist"
rm -rf "/Library/PreferencePanes/KeyRemap4MacBook.prefPane"

exit 0
