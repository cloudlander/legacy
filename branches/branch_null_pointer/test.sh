#!/bin/bash

for src in `ls *.curry`
do
	prefix=${src%%.curry}
	echo "TESTING $prefix" | tee -a log
#	./cuc < $src | ./cyg.sh > $prefix.s
	./cuc < $src  > $prefix.s
	gcc $prefix.s thunk.c -o $prefix
#	./cuc < $src 2> $prefix.output
	./$prefix | tee $prefix.output
	cat $prefix.output >> log
	diff $prefix.output $prefix.out >> log
	echo | tee -a log
done
