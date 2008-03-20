#!/bin/sh

for dir in EX EY EZ HX HY HZ
do
  rm -rf ${dir}_MAP/* ${dir}_3D/*
  echo "Delete all files in $dir? CTRL-C to cancel"
  read
  rm -rf $dir/*
done
