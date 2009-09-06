#!/bin/sh

basedir=`dirname $0`

for f in `find $basedir/../../* -name 'project.pbxproj'`; do
    echo "Check $f"
    "$basedir/xcodeproj.rb" < $f
done
