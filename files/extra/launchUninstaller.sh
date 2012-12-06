#!/bin/sh

TMPDIR=`/usr/bin/mktemp -d /tmp/KeyRemap4MacBook_uninstaller.XXXXXX` || exit 1
/usr/bin/rsync -a /Applications/KeyRemap4MacBook.app/Contents/Applications/KeyRemap4MacBookUninstaller.app "$TMPDIR"
/Applications/KeyRemap4MacBook.app/Contents/Library/extra/setpermissions.sh "$TMPDIR/KeyRemap4MacBookUninstaller.app"
/usr/bin/open "$TMPDIR/KeyRemap4MacBookUninstaller.app"
