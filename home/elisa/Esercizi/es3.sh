#!/bin/bash
echo "Utilizzo di questo script: $0"

function crea_tar()
{	
	tar -czvf $1.tar.gz /home/elisa/Esercizi
	echo "good"
	if ! [ -e $1.tar.gz ]; then
		echo "errore"
	else
		file_t=$1.tar.gz
		ls -d $file_t
	fi
}

if [ $2 ]; then
	echo "Errore"
elif ! [ -d $1 ]; then
	echo "Errore"
elif [ -d $1 ]; then
	if ! [ -e $1.tar.gz ]; then
		crea_tar
	else	
		echo "La cartella esiste gia', vuoi sovrascriverla? s/n"
		read ris
		case $ris in
		s) 
			rmdir $1.tar.gz
			crea_tar
			;;
		*) 	
			;;
		esac
	fi
			
	
		
	#else 
		#ELENCO=$ tar -tvf "$1".tar.gz
		#echo $ELENCO
fi

#if  ! [ find .Esercizi  '*.tar.gz' -exec tar]; then
		#echo "Errore"
#ELENCO=$ tar -tvf "$1".tar.gz
		#echo $ELENCO

