#!/bin/sh

make clean all
version=`ruby -e '/<key>CFBundleShortVersionString<\/key>\s*<string>(.+)<\/string>/ =~ readlines.join("\n"); print $1, "\n"' Info.plist`

(cd build/Release; zip -r ThreeFingerRelativeToScroll-$version.app.zip ThreeFingerRelativeToScroll.app; mv *.zip ../..)
