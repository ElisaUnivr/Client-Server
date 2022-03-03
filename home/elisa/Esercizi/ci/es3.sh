#!/bin/bash
echo "Utilizzo di questo script: $0"

if [ $2 ]; then
	echo "Errore"
elif ! [ -d $1 ]; then
	echo "Errore"
elif [ -d $1 ]; then
	tar -czvf $1.tar.gz 
	if ! [ -e $1.tar.gz ]; then
		echo "errore"
	
		#tar -xvvf $1.tar.gz
		#cat $1.tar.gz
	fi
	
	#else 
		#ELENCO=$ tar -tvf "$1".tar.gz
		#echo $ELENCO
fi

#if  ! [ find .Esercizi  '*.tar.gz' -exec tar]; then
		#echo "Errore"
#ELENCO=$ tar -tvf "$1".tar.gz
		#echo $ELENCO
