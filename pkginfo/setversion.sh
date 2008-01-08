#!/bin/sh

basedir=`dirname $0`
version=$(cat $basedir/../version)
sed "s|PKGVERSION|$version|g" Info.plist.tmpl > Info.plist
