#!/bin/sh

TMPDIR=`/usr/bin/mktemp -d /tmp/Karabiner_uninstaller.XXXXXX` || exit 1
/usr/bin/rsync -a /Applications/Karabiner.app/Contents/Applications/KarabinerUninstaller.app "$TMPDIR"
/Applications/Karabiner.app/Contents/Library/extra/setpermissions.sh "$TMPDIR/KarabinerUninstaller.app"
/usr/bin/open "$TMPDIR/KarabinerUninstaller.app"
