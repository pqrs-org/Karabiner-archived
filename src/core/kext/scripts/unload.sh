#!/bin/bash

sudo /bin/echo

sudo killall -USR1 Karabiner

for i in 0 1 2 3 4 5; do
    sleep 0.5 && echo -n "."
done

sudo kextunload -b org.pqrs.driver.Karabiner

exit 0
