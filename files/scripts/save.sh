#!/bin/sh
PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

tmpfile=`mktemp /tmp/KeyRemap4MacBook.XXXXXX` || exit 1
basedir="/Library/org.pqrs/KeyRemap4MacBook"
outfile="$basedir/scripts/sysctl.sh"

cat > "$tmpfile" <<EOF
#!/bin/sh

PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

i=0
while [ -z "\`sysctl -n keyremap4macbook.version\`" ]; do
  echo "Waiting KeyRemap4MacBook kext setup ...(\$i)"
  sleep 3
  i=\`expr \$i + 1\`
  if [ \$i -gt 9 ]; then
    echo "Giveup waiting"
    exit 1
  fi
done

EOF

for key in `sysctl keyremap4macbook 2>&1 | grep -oE '^keyremap4macbook.(general|remap|option|repeat|pointing).[^:]+'`; do
    value=`sysctl -n $key`
    if [ "$value" != "0" ]; then
        echo "sysctl -w $key=$value" >> "$tmpfile"
    fi
done

chmod 755 "$tmpfile"
mv -f "$tmpfile" "$outfile"

exit 0
