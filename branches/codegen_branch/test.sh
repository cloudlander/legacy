for src in `ls *.curry`

do

	prefix=${src%%.curry}

	echo "TESTING $prefix"

	./cuc < $src | cyg.sh > $prefix.s

	gcc $prefix.s thunk.c -o $prefix

	./$prefix | tee log

done

