#!/bin/sh

/Applications/Karabiner.app/Contents/Library/bin/karabiner \
    reloadxml
/usr/bin/osascript -e 'display notification "Reloaded" with title "Karabiner"';

exit 0
