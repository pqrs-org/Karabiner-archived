#!/bin/sh

version=$(cat version)

packagemaker=/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker
pkgName="KeyRemap4MacBook.pkg"
archiveName="KeyRemap4MacBook-${version}"

make clean build || exit $?

# --------------------------------------------------
# http://developer.apple.com/documentation/Darwin/Conceptual/KEXTConcept/KEXTConceptPackaging/packaging_kext.html
echo "Copy Files"

rm -rf pkgroot
mkdir -p pkgroot

basedir="/Library/org.pqrs/KeyRemap4MacBook"

mkdir -p                                                "pkgroot/$basedir"
cp -R src/core/kext/build/Release/KeyRemap4MacBook.kext "pkgroot/$basedir"
cp -R files/scripts                                     "pkgroot/$basedir"

mkdir -p                               "pkgroot/$basedir/extra"
cp -R pkginfo/Resources/preflight      "pkgroot/$basedir/extra/uninstall_core.sh"
cp -R files/extra/launchUninstaller.sh "pkgroot/$basedir/extra/"
cp -R files/extra/setpermissions.sh    "pkgroot/$basedir/extra/"
cp -R files/extra/uninstall.sh         "pkgroot/$basedir/extra/"

mkdir -p                  "pkgroot/Library"
cp -R files/LaunchDaemons "pkgroot/Library"
cp -R files/LaunchAgents  "pkgroot/Library"

mkdir -p                                                                        "pkgroot/$basedir/bin"
cp -R "src/bin/dump_xml_compiler_result/build/Release/dump_xml_compiler_result" "pkgroot/$basedir/bin"

mkdir -p                                                                                    "pkgroot/$basedir/app"
cp -R "src/core/server/build/Release/KeyRemap4MacBook.app"                                  "pkgroot/$basedir/app"
cp -R "src/util/KeyDump/build/Release/KeyDump.app"                                          "pkgroot/$basedir/app"
cp -R "src/util/multitouchextension/build/Release/KeyRemap4MacBook_multitouchextension.app" "pkgroot/$basedir/app"
cp -R "src/util/cli/build/Release/KeyRemap4MacBook_cli.app"                                 "pkgroot/$basedir/app"
cp -R "src/util/uninstaller/automator/KeyRemap4MacBookUninstaller.app"                      "pkgroot/$basedir/app"

mkdir -p                                                          "pkgroot/Library/PreferencePanes"
cp -R "src/util/prefpane/build/Release/KeyRemap4MacBook.prefPane" "pkgroot/Library/PreferencePanes"

# Setting file permissions.
#
# Note:
#   If target files are already exists in system disk,
#   PackageMaker uses their permissions.
#
#   For example:
#     If /Library/org.pqrs permission is 0777 by accidental reasons,
#     the directory permission will be 0777 in Archive.bom
#     even if we set this directory permission to 0755 by setpermissions.sh.
#
#   Then, we need to repair file permissions in postflight script.
#   Please also see postflight.
#
sh "files/extra/setpermissions.sh" pkgroot

# --------------------------------------------------
echo "Exec PackageMaker"

rm -rf $archiveName/$pkgName
mkdir $archiveName

# Note: Don't add --no-recommend option.
# It breaks /Library permission.
# (It overwrites /Library permission with pkgroot/Library permission.)
# - Mac OS X 10.6: /Library is 1775
# - Mac OS X 10.7: /Library is 0755
$packagemaker \
    --root pkgroot \
    --info pkginfo/Info.plist \
    --resources pkginfo/Resources \
    --title "KeyRemap4MacBook $version" \
    --no-relocate \
    --out $archiveName/$pkgName

# --------------------------------------------------
echo "Fix Archive.bom"
ruby pkginfo/fixbom.rb $archiveName/$pkgName/Contents/Archive.bom pkgroot/

# --------------------------------------------------
echo "Make Archive"

# Note:
# Some third vendor archiver fails to extract zip archive.
# Therefore, we use dmg instead of zip.

rm -f $archiveName.dmg
hdiutil create -nospotlight $archiveName.dmg -srcfolder $archiveName
rm -rf $archiveName
chmod 644 $archiveName.dmg
