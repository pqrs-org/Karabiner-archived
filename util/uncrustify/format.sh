#!/bin/sh

basedir=`dirname $0`

find ../../* -name '*.cpp' -type f | xargs uncrustify -c "$basedir/uncrustify.cfg" --no-backup
find ../../* -name '*.hpp' -type f | xargs uncrustify -c "$basedir/uncrustify.cfg" --no-backup
find ../../* -name '*.m' -type f | xargs uncrustify -c "$basedir/uncrustify.cfg" --no-backup
find ../../* -name '*.h' -type f | xargs uncrustify -c "$basedir/uncrustify.cfg" --no-backup -l OC
