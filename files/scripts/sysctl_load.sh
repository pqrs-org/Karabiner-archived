#!/bin/sh

PATH=/bin:/sbin:/usr/bin:/usr/sbin; export PATH

#----------------------------------------
i=0
while [ -z "`sysctl -n keyremap4macbook.version`" ]; do
  echo "Waiting KeyRemap4MacBook kext setup ...($i)"
  sleep 3
  i=`expr $i + 1`
  if [ $i -gt 9 ]; then
    echo "Giveup waiting"
    exit 1
  fi
done

#----------------------------------------
/Library/org.pqrs/KeyRemap4MacBook/scripts/sysctl_reset.sh
/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_save --load
