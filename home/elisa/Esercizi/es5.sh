#!/bin/bash

read file1 file2
echo "$file1"
echo "$file2"
if ! [ -f $file1 ] ; then
	exit 0
else
	echo "	1. rimuovere entrambi i fie \n
		2. archiviare entrambi \n
		3. appendere il file 1 al file 2 \n
		4. esci"
	read numero
	case $numero in
	1)
		echo "Vuoi rimuovere? s/n"
		read ris
		case $ris in
		s)
			rm $file1 $file2
			;;
		*)
		esac
		;;
	2)	
		echo "archivia"	
		tar -cvzf $file1$file2.tar.gz $file1 $file2
		;;
	3)	
		echo "appedere"
		cat $file2 >> $file1
		;;
	4)	
		echo "esci"
		;;
	esac
fi
