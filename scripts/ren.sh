#!/bin/bash
file=$1
pat1=$2
pat2=$3
dest=${file%%$pat1}$pat2
echo Renaming $file to $dest
mv $file $dest

