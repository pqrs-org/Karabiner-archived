#!/bin/sh

TMPDIR=`mktemp -d /tmp/uninstaller.XXXXXX` || exit 1
rsync -a /Library/org.pqrs/KeyRemap4MacBook/app/uninstaller.app "$TMPDIR"
open "$TMPDIR/uninstaller.app"
