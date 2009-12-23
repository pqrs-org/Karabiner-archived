#!/bin/sh

version=$(cat version)

packagemaker=/Developer/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker
pkgName="KeyRemap4MacBook.pkg"
archiveName="KeyRemap4MacBook-${version}.pkg.tar.gz"

make clean build || exit $?

# --------------------------------------------------
# http://developer.apple.com/documentation/Darwin/Conceptual/KEXTConcept/KEXTConceptPackaging/packaging_kext.html
echo "Copy Files"

sudo rm -rf pkgroot
sudo mkdir -p pkgroot

basedir="/Library/org.pqrs/KeyRemap4MacBook"
sudo mkdir -p "pkgroot/$basedir/tmp"
sudo cp -R src/core/kext/build/Release/KeyRemap4MacBook.kext "pkgroot/$basedir"
sudo cp -R files/prefpane "pkgroot/$basedir"
sudo cp -R files/scripts "pkgroot/$basedir"
sudo cp -R files/share "pkgroot/$basedir"

sudo mkdir -p "pkgroot/$basedir/extra"
sudo cp -R pkginfo/Resources/preflight "pkgroot/$basedir/extra/uninstall.sh"
sudo cp -R files/extra/launchUninstaller.sh "pkgroot/$basedir/extra/"

sudo mkdir -p "pkgroot/Library"
sudo cp -R files/LaunchDaemons pkgroot/Library
sudo cp -R files/LaunchAgents pkgroot/Library

sudo mkdir -p "pkgroot/$basedir/app"
sudo cp -R "src/core/server/build/Release/KeyRemap4MacBook.app" "pkgroot/$basedir/app"
sudo cp -R "src/util/statusbar/build/Release/KeyRemap4MacBook_statusbar.app" "pkgroot/$basedir/app"
sudo cp -R "src/util/uninstaller/build/Release/uninstaller.app" "pkgroot/$basedir/app"

sudo mkdir -p "pkgroot/$basedir/bin"
sudo cp src/bin/sysctl_ctl/build/Release/KeyRemap4MacBook_sysctl_ctl "pkgroot/$basedir/bin"
sudo cp src/bin/sysctl_reset/build/Release/KeyRemap4MacBook_sysctl_reset "pkgroot/$basedir/bin"
sudo cp src/bin/sysctl_set/build/Release/KeyRemap4MacBook_sysctl_set "pkgroot/$basedir/bin"

sudo mkdir -p "pkgroot/Library/PreferencePanes"
sudo cp -R "src/util/prefpane/build/Release/KeyRemap4MacBook.prefPane" "pkgroot/Library/PreferencePanes"

sudo find pkgroot -type d -print0 | xargs -0 sudo chmod 755
sudo find pkgroot -type f -print0 | xargs -0 sudo chmod 644
sudo find pkgroot -name '*.sh' -print0 | xargs -0 sudo chmod 755
sudo chmod 4755 pkgroot/$basedir/bin/KeyRemap4MacBook_sysctl_reset
sudo chmod 4755 pkgroot/$basedir/bin/KeyRemap4MacBook_sysctl_set
sudo chmod 755 pkgroot/$basedir/bin/KeyRemap4MacBook_sysctl_ctl
sudo chmod 755 pkgroot/$basedir/app/KeyRemap4MacBook.app/Contents/MacOS/KeyRemap4MacBook
sudo chmod 755 pkgroot/$basedir/app/KeyRemap4MacBook_statusbar.app/Contents/MacOS/KeyRemap4MacBook_statusbar
sudo chmod 755 pkgroot/$basedir/app/uninstaller.app/Contents/MacOS/uninstaller
sudo chown -R root:wheel pkgroot

sudo chmod 1775 pkgroot/Library
sudo chown root:admin pkgroot/Library

# --------------------------------------------------
echo "Exec PackageMaker"

rm -rf $pkgName
sudo $packagemaker -build \
    -p $pkgName \
    -f pkgroot \
    -ds \
    -r pkginfo/Resources \
    -i pkginfo/Info.plist \
    -d pkginfo/Description.plist

# --------------------------------------------------
echo "Make Archive"

sudo chown -R root:wheel $pkgName
sudo tar zcf $archiveName $pkgName
sudo rm -rf $pkgName
tar zxf $archiveName

# --------------------------------------------------
echo "Cleanup"
sudo rm -rf pkgroot
make -C src clean
