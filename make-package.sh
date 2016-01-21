#!/bin/sh

# Package build into a signed .dmg file

# set $GEM_HOME/bin/ for CocoaPods.
PATH="/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:$GEM_HOME/bin"; export PATH

version=$(cat version)

make clean build || exit $?

# --------------------------------------------------
# https://developer.apple.com/library/mac/documentation/Darwin/Conceptual/KEXTConcept/KEXTConceptPackaging/packaging_tutorial.html
echo "Copy Files"

rm -rf pkgroot
mkdir -p pkgroot

mkdir -p "pkgroot/Applications"
cp -R "src/core/server/build/Release/Karabiner.app" "pkgroot/Applications"

basedir="pkgroot/Applications/Karabiner.app/Contents/Applications"
mkdir -p "$basedir"
for d in \
    src/util/AXNotifier/build/Release/Karabiner_AXNotifier.app \
    src/util/EventViewer/build/Release/EventViewer.app \
    src/util/multitouchextension/build/Release/Karabiner_multitouchextension.app \
    ;
do
    cp -R "$d" "$basedir"
done

basedir="pkgroot/Applications/Karabiner.app/Contents/Applications/Utilities"
mkdir -p "$basedir"
cp -R files/Utilities/*.app "$basedir"

basedir="pkgroot/Applications/Karabiner.app/Contents/Library/bin"
mkdir -p "$basedir"
cp -R src/util/cli/build/Release/karabiner "$basedir"

basedir="pkgroot/Applications/Karabiner.app/Contents/Library/utilities/bin"
mkdir -p "$basedir"
cp -R src/util/warp-mouse-cursor-position/build/Release/warp-mouse-cursor-position "$basedir"
cp -R src/util/post-hid-event/build/Release/post-hid-event "$basedir"

basedir="pkgroot/Applications/Karabiner.app/Contents/Library/vendor/bin"
mkdir -p "$basedir"
cp -R src/vendor/blueutil/build/Release/blueutil "$basedir"

mkdir -p                  "pkgroot/Library"
cp -R files/LaunchDaemons "pkgroot/Library"

basedir="pkgroot/Library/Application Support/org.pqrs/Karabiner"
mkdir -p "$basedir"
cp -R src/core/kext/build/Release/Karabiner.kext "$basedir/Karabiner.signed.kext"

cp -R pkginfo/Scripts/preinstall "$basedir/uninstall_core.sh"
for f in \
    files/extra/setpermissions.sh \
    files/extra/startup.sh \
    files/extra/uninstall.sh \
    files/extra/uninstaller.applescript \
    ;
do
    cp -R "$f" "$basedir"
done

# Sign with Developer ID
bash files/extra/codesign.sh "pkgroot"

# Setting file permissions.
#
# Note:
#   If target files are already exists in system disk,
#   PackageMaker uses their permissions.
#
#   For example:
#     If /Applications/Karabiner.app permission is 0777 by accidental reasons,
#     the directory permission will be 0777 in Archive.bom
#     even if we set this directory permission to 0755 by setpermissions.sh.
#
#   Then, we need to repair file permissions in postinstall script.
#   Please also see postinstall.
#
sh "files/extra/setpermissions.sh" pkgroot
sh "files/extra/setpermissions.sh" pkginfo
chmod 755 \
    pkginfo/Scripts/postinstall \
    pkginfo/Scripts/preinstall \
    pkginfo/fixbom.rb

# --------------------------------------------------
echo "Create pkg"

pkgName="Karabiner.sparkle_guided.pkg"
pkgIdentifier="org.pqrs.driver.Karabiner"
archiveName="Karabiner-${version}"

rm -rf $archiveName
mkdir $archiveName

pkgbuild \
    --root pkgroot \
    --component-plist pkginfo/pkgbuild.plist \
    --scripts pkginfo/Scripts \
    --identifier $pkgIdentifier \
    --version $version \
    --install-location "/" \
    $archiveName/Installer.pkg

echo "Fix Archive.bom"
pkgutil --expand $archiveName/Installer.pkg $archiveName/expanded
ruby pkginfo/fixbom.rb $archiveName/expanded/Bom pkgroot/
pkgutil --flatten $archiveName/expanded $archiveName/Installer.pkg
rm -r $archiveName/expanded


productbuild \
    --distribution pkginfo/Distribution.xml \
    --package-path $archiveName \
    $archiveName/$pkgName

rm -f $archiveName/Installer.pkg

# --------------------------------------------------
echo "Sign with Developer ID"
bash files/extra/codesign-pkg.sh $archiveName/$pkgName

# --------------------------------------------------
echo "Make Archive"

# Note:
# Some third vendor archiver fails to extract zip archive.
# Therefore, we use dmg instead of zip.

rm -f $archiveName.dmg
hdiutil create -nospotlight $archiveName.dmg -srcfolder $archiveName
rm -rf $archiveName
chmod 644 $archiveName.dmg
