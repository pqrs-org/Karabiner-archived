#!/bin/sh

sudo cp -R build/Release/SetCapsDelay.kext /tmp/
sudo kextutil -t /tmp/SetCapsDelay.kext
