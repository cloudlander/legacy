src=$1

param=$2

prefix=${src%.*}



echo "./cuc $param < $src > .$prefix.s"

./cuc $param < $src |  cyg.sh > .$prefix.s

if [ $? -eq 0 ]

then

	echo "gcc .$prefix.s thunk.c -o $prefix.exe"

	gcc .$prefix.s thunk.c -o $prefix.exe

	chmod +x $prefix.exe

	if [ $? -eq 0 ]

	then

		echo "runing $prefix.exe"

		./$prefix.exe

	fi

fi

rm -f .$prefix.s		 

