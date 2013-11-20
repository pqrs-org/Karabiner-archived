#!/bin/sh

PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

# --------------------------------------------------
sudo rm -rf /tmp/KeyRemap4MacBook
mkdir /tmp/KeyRemap4MacBook

cp -R build/Release/KeyRemap4MacBook.kext /tmp/KeyRemap4MacBook/KeyRemap4MacBook.signed.kext
bash ../../../files/extra/codesign.sh /tmp/KeyRemap4MacBook
sudo chown -R root:wheel /tmp/KeyRemap4MacBook

sudo kextutil -t /tmp/KeyRemap4MacBook/KeyRemap4MacBook.signed.kext
