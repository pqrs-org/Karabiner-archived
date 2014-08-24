#!/bin/sh

basedir=`dirname $0`

############################################################
# xcodeproj
for f in `find $basedir/../../* -name 'project.pbxproj' ! -ipath '*/Pods/*'`; do
    echo "Check $f"

    plutil -convert json -o - "$f" | "$basedir/xcodeproj.rb"  || exit 1
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
