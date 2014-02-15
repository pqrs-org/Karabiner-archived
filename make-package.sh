#!/bin/sh

PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

version=$(cat version)

make clean build || exit $?

# --------------------------------------------------
# http://developer.apple.com/documentation/Darwin/Conceptual/KEXTConcept/KEXTConceptPackaging/packaging_kext.html
echo "Copy Files"

rm -rf pkgroot
mkdir -p pkgroot

mkdir -p "pkgroot/Applications"
cp -R "src/core/server/build/Release/KeyRemap4MacBook.app" "pkgroot/Applications"

basedir="pkgroot/Applications/KeyRemap4MacBook.app/Contents/Applications"
mkdir -p "$basedir"
for d in \
    src/util/EventViewer/build/Release/EventViewer.app \
    src/util/cli/build/Release/KeyRemap4MacBook_cli.app \
    src/util/multitouchextension/build/Release/KeyRemap4MacBook_multitouchextension.app \
    src/util/uninstaller/automator/KeyRemap4MacBookUninstaller.app \
    ;
do
    cp -R "$d" "$basedir"
done

basedir="pkgroot/Applications/KeyRemap4MacBook.app/Contents/Applications/Utilities"
mkdir -p "$basedir"
cp -R files/Utilities/*.app "$basedir"

basedir="pkgroot/Applications/KeyRemap4MacBook.app/Contents/Library"
mkdir -p "$basedir"
cp -R src/core/kext/build/Release/KeyRemap4MacBook.kext "$basedir/KeyRemap4MacBook.signed.kext"

basedir="pkgroot/Applications/KeyRemap4MacBook.app/Contents/Library/bin"
mkdir -p "$basedir"
cp -R src/bin/kextload/build/Release/kextload "$basedir"

basedir="pkgroot/Applications/KeyRemap4MacBook.app/Contents/Library/extra"
mkdir -p "$basedir"
cp -R pkginfo/Scripts/preinstall "$basedir/uninstall_core.sh"
for f in \
    files/extra/launchUninstaller.sh \
    files/extra/setpermissions.sh \
    files/extra/uninstall.sh \
    ;
do
    cp -R "$f" "$basedir"
done

basedir="pkgroot/Applications/KeyRemap4MacBook.app/Contents/Library/vendor/bin"
mkdir -p "$basedir"
cp -R src/vendor/blueutil/build/Release/blueutil "$basedir"

basedir="pkgroot/Library"
mkdir -p "$basedir"
for d in \
    files/LaunchAgents \
    ;
do
    cp -R "$d" "$basedir"
done

# Sign with Developer ID
bash files/extra/codesign.sh pkgroot

# Setting file permissions.
#
# Note:
#   If target files are already exists in system disk,
#   PackageMaker uses their permissions.
#
#   For example:
#     If /Applications/KeyRemap4MacBook.app permission is 0777 by accidental reasons,
#     the directory permission will be 0777 in Archive.bom
#     even if we set this directory permission to 0755 by setpermissions.sh.
#
#   Then, we need to repair file permissions in postinstall script.
#   Please also see postinstall.
#
chmod 4755 pkgroot/Applications/KeyRemap4MacBook.app/Contents/Library/bin/kextload
sh "files/extra/setpermissions.sh" pkgroot
sh "files/extra/setpermissions.sh" pkginfo
chmod 755 \
    pkginfo/Scripts/postinstall \
    pkginfo/Scripts/preinstall \
    pkginfo/fixbom.rb

# --------------------------------------------------
echo "Create pkg"

pkgName="KeyRemap4MacBook.pkg"
pkgIdentifier="org.pqrs.driver.KeyRemap4MacBook"
archiveName="KeyRemap4MacBook-${version}"

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
