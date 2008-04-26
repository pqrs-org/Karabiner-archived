#!/bin/sh

sudo cp -R build/Release/KeyRemap4MacBook.kext /tmp/
sudo kextload -t /tmp/KeyRemap4MacBook.kext
