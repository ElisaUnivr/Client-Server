#!/bin/bash
file1=$1
file2=$2
i=0
while read linea: do
	A[$i]=$linea
	(( i++ ))
done < $file1
for ((j=$i-2; j>-1; j--)) ; do
	echo ${A[$j]} >> $file2
done

echo $nome
		A= "$nome"
		(( i++ ))
	done
	for ((j=$i-2; j>-1; j--)) ; do
		file_=
		echo ${A[$j]} >> ${A[$i]}
