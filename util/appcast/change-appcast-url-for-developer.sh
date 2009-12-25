#!/bin/sh

plist=/Library/org.pqrs/KeyRemap4MacBook/app/KeyRemap4MacBook.app/Contents/Info.plist

sed 's|appcast.xml|appcast-devel.xml|' $plist > tmp
sudo mv tmp $plist
