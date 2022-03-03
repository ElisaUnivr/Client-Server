#!/bin/bash
echo "Utilizzo di questo script: $0"

if [ $2 ]; then
	echo "Errore"
fi
if [ -d "$1" ]; then
	echo "creo l'archivio "$1".tar.gz"
	tar -cvzfW "$1".tar.gz /home/elisa
else
	
	echo "Errore"
fi
#if [ tar -tfw ${a.tar.gz} ]; then
#	echo "esiste"
#fi
	
