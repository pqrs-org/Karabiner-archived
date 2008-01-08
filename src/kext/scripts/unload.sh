#!/bin/sh

sudo `dirname $0`/save.sh
sudo kextunload -b org.pqrs.driver.KeyRemap4MacBook
