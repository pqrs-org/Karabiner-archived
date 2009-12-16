#!/bin/sh

basedir=`dirname $0`

for f in `find $basedir/../../* -name 'project.pbxproj'`; do
    echo "Check $f"

    if [ "$(basename $(dirname $f))" = "ThreeFingerRelativeToScroll.xcodeproj" ]; then
        # ThreeFingerRelativeToScroll doesn't use GC to prevent malloc error in private API.
        "$basedir/xcodeproj.rb" GCC_ENABLE_OBJC_GC < $f || exit 1
    else
        "$basedir/xcodeproj.rb" < $f || exit 1
    fi
done
