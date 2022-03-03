#!/bin/bash

if ! [ $3 ]; then
	echo "Numero di file non sufficiente"
else
	i=0
	for nome in $@; do
		array+=($nome)
	done 
	declare -p array
	
	n=${#array[@]}

	for ((j=$n-1; j>-1; j--)) ; do
						
			cat ${array[$j]} >> ${array[n-1]}
			
	done
fi
	
	
