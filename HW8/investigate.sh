#!/bin/bash

for x in "$(ls /usr/bin | awk 'NR%101==your_uid%101' $1)"; do
    y=`which $x`
    ldd $y
done 
