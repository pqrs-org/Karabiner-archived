#!/bin/sh

TMPDIR=`/usr/bin/mktemp -d /tmp/Karabiner_uninstaller.XXXXXX` || exit 1
/usr/bin/rsync -a /Applications/Karabiner.app/Contents/Applications/KarabinerUninstaller.app "$TMPDIR"
sh '/Library/Application Support/org.pqrs/Karabiner/setpermissions.sh' "$TMPDIR/KarabinerUninstaller.app"
/usr/bin/open "$TMPDIR/KarabinerUninstaller.app"
