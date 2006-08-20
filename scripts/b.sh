#!/bin/bash

head=`/usr/bin/head -n 1 $1`

if [[ $head == "#define printf" ]]
then
	echo "OK"
	let num=`wc -l $1 | awk {'print $1'}`-1
	/usr/bin/tail -n $num $1 > $1.bak
	mv $1.bak $1
else
	sed -e '1 i #define printf' $1 > $1.bak
	mv $1.bak $1
fi
