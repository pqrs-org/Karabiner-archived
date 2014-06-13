#!/bin/sh

PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

# --------------------------------------------------
sudo rm -rf /tmp/Karabiner
mkdir /tmp/Karabiner

cp -R build/Release/Karabiner.kext /tmp/Karabiner/Karabiner.signed.kext
bash ../../../files/extra/codesign.sh /tmp/Karabiner
sudo chown -R root:wheel /tmp/Karabiner

sudo kextutil -t /tmp/Karabiner/Karabiner.signed.kext
