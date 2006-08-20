for lf in `ls *.c`
do
	pre=${lf%%\.c}
	echo $pre
	echo "####  $pre.c ####\n" 
#	cp $pre.c cs_$pre.c
#	cat $pre.c >> all
#	echo "\n----  lf ----\n" >> all
	perl ls_and_do.pl $pre.c
#	echo "\n#### $pre.lf end ####\n" >> all
done
