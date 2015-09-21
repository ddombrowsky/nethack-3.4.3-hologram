#!/bin/bash

VAL=$1
COUNT=$2

if [ -z "$VAL" -o -z "$COUNT" ] ; then
    echo "Usage $0 <value_per> <number_of_entries>"
    exit 1
fi


while [ $COUNT -gt 0 ] ; do
    ((COUNT--))
    newkey=`radiosharesd getnewaddress nethack`
    newprivkey=`radiosharesd dumpprivkey $newkey`
    if ! radiosharesd sendtoaddress $newkey $VAL > /dev/null ; then
        continue
    fi
    echo $newkey:$newprivkey
done
