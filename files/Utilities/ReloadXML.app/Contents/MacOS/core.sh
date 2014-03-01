#!/bin/sh

/Applications/KeyRemap4MacBook.app/Contents/Applications/KeyRemap4MacBook_cli.app/Contents/MacOS/KeyRemap4MacBook_cli \
    reloadxml
/usr/bin/osascript -e 'display notification "Reloaded" with title "KeyRemap4MacBook"';

exit 0
