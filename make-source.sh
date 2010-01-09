#!/bin/sh

name="$(basename $(pwd))-$(cat version)"
tmpdir=$(mktemp -d "/tmp/$name.XXXXXX")
workdir="$tmpdir/$name"

# copy
rsync -a . $workdir

# cleanup
(cd $workdir; make clean)
rm -rf $workdir/.hg*
find $workdir -name '.DS_Store' -print0 | xargs -0 rm -f

# archive
tar czf $name.tar.gz -C $tmpdir $name
chmod 644 $name.tar.gz

# purge tmpdir
rm -rf $tmpdir
