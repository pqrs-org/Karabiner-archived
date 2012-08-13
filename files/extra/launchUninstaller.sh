#!/bin/sh

TMPDIR=`/usr/bin/mktemp -d /tmp/KeyRemap4MacBook_uninstaller.XXXXXX` || exit 1
/usr/bin/rsync -a /Library/org.pqrs/KeyRemap4MacBook/app/uninstaller.app "$TMPDIR"
/Library/org.pqrs/KeyRemap4MacBook/extra/setpermissions.sh "$TMPDIR/uninstaller.app"
/usr/bin/open "$TMPDIR/uninstaller.app"
