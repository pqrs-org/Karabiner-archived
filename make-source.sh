#!/bin/sh

name="$(basename $(pwd))-$(cat version)"
tmpdir=$(mktemp -d "/tmp/$name.XXXXXX")
workdir="$tmpdir/$name"

# copy
rsync -a . --exclude '.git/' --exclude '.gitignore' $workdir

# cleanup
(cd $workdir; make clean)
find $workdir -name '.DS_Store' -print0 | xargs -0 rm -f

# archive
(cd $tmpdir; zip -X -r -9 --symlinks $name.zip $name)
mv $tmpdir/$name.zip .
chmod 644 $name.zip

# purge tmpdir
rm -rf $tmpdir
