#!/bin/sh

basedir=`dirname $0`

############################################################
# xcodeproj
for f in `find $basedir/../../* -name 'project.pbxproj'`; do
    echo "Check $f"
    "$basedir/xcodeproj.rb" < $f || exit 1
done

############################################################
# Info.plist.tmpl
for f in `find $basedir/../../* -name 'Info.plist.tmpl'`; do
    echo "Check $f"

    case "$(basename $(dirname $f))/$(basename $f)" in
        pkginfo/Info.plist.tmpl)
            "$basedir/plist.rb" CFBundleIconFile < $f || exit 1
            ;;
        kext/Info.plist.tmpl)
            "$basedir/plist.rb" CFBundleIconFile < $f || exit 1
            ;;
        prefpane/Info.plist.tmpl)
            "$basedir/plist.rb" CFBundleIconFile < $f || exit 1
            ;;
        *)
            "$basedir/plist.rb" < $f || exit 1
            ;;
    esac
done
