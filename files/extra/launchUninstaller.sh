#!/bin/sh

TMPDIR=`/usr/bin/mktemp -d /tmp/KeyRemap4MacBook_uninstaller.XXXXXX` || exit 1
/usr/bin/rsync -a /Library/org.pqrs/KeyRemap4MacBook/app/uninstaller.app "$TMPDIR"
/usr/bin/open "$TMPDIR/uninstaller.app"
