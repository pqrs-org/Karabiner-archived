#!/bin/sh

name="$(basename $(pwd))-$(cat version)"
git archive --format=zip --prefix="${name}/" HEAD > $name.zip
chmod 644 $name.zip
