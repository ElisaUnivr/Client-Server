
#define _POSIX_C_SOURCE 200112L


#include "util.h"

/*
void genera_thread(int sock_file_d);
void *thread_gestisci_client(void*arg);
void *thread_gestisci_risposta(void *arg);
void *thread_magazzino(void *arg);
void compra(char *buf);
int pagamento_(char *buf);*/

//la dichiaro globali in modo da utilizzarle in piu' funzioni
int sock_file_d;
int array_libri[4]={0};//contiene il numero di libri a disposizione
int importo,occupato=1;
int fine=1;
int ordine_libri=1;//la setto a 0 quando controllo se ci sono tutti i libri richiesti dal client


char libri_disponibili[]="Salve! Ecco la lista dei libri disponibili: \na)Il Codice da Vinci, di Dan Brown \nb)Harry Potter e la Pietra Filosofale,di J.K. Rowling \nc)Lo Hobbit,di J.R.R. Tolkien \nd)L'alchimista, di Paulo Coelho\ne)Don Chisciotte della Mancha, di Miguel de Cervantes\nSe vuoi fare un ordine digita: ordine\n";
char ordinare[]="Ora dimmi il libro da ordinare e la quantita'";

//funzione per modificare i caratteri del messaggio del client
void compra(char *x)
{	

	importo=0;
	//separo il tipo di e il numero di libri che il client vuole comprare
	
	const char delimitatori[]=" ";
	char *token=strtok(x,delimitatori); 
	//char stringa=(char) x; 
	char *numero=" ";
	
	int num;
	int lettera=0;
	while(token != NULL){
		
		//per il numero di libri
		numero=token+1;
		num=atoi(numero);
		ordine_libri=1;
		//per il tipo di libro
		if(strncmp(token,"a",1)==0){
		
			lettera=0;
		}else if(strncmp(token,"b",1)==0){
		
			lettera=1;
		}else if(strncmp(token,"c",1)==0){
		
			lettera=2;
		}else if(strncmp(token,"d",1)==0){
		
			lettera=3;
		}else if(strncmp(token,"e",1)==0){
		
			lettera=4;
		
		}
		
		if(array_libri[lettera]>= num ){
			
			array_libri[lettera]=array_libri[lettera]-num;
			importo=importo+(5*num); 
			ordine_libri=0;
			
			
		}else{	
			char inizio[50]="Abbiamo solo ";
			
			char var[50];
			sprintf(var, "%d", array_libri[lettera]);
			
			strcat(inizio,var);

			if(lettera==0){
				
				strcat(inizio,"'Il codice da Vinci'");
				strcpy(x,inizio);

			}else if(lettera==1){
				
				strcat(inizio,"'Harry Potter e la Pietra Filosofale'");
				strcpy(x,inizio);
			
			}else if(lettera==2){
				strcat(inizio,"'Lo Hobbit'");
				strcpy(x,inizio);
			
			}else if(lettera==3){
				strcat(inizio,"'L'alchimista'");
				strcpy(x,inizio);
			
			}else if(lettera==4){
				strcat(inizio,"'Don Chisciotte della Mancha'");
				strcpy(x,inizio);
			
			}
			
			
			return;

		}
				
			
			token=strtok(NULL,delimitatori);
			
		
	}
	
	strcpy(x,"ORDINE EFFETTUATO. Ora inserisci i soldi per pagare.\n");
	return;
}

//mi ritorna 1 se il client mi da abbastanza soldi per pagare, 0 altrimenti
int pagamento_(char *x){
	int n=atoi(x);
	if(n>=importo){
		strcpy(x,"PAGAMENTO EFFETTUATO.Grazie di averci scelto! I suoi libri sono in arrivo!\n");
		return 1;
	}else{
		strcpy(x,"Non mi hai dato tutti i soldi!!\n");
			
		return 0;
	}

}



void cleanup() 
{
    if (unlink(SOCKNAME) < 0)
    {
        perror("unlink");
    }
}


//incrementa il numero di libri periodicamente
void *thread_magazzino(void *arg){
	
	srand(time(0));
	int i=0;
	int j=0;

	while(1){
		i=rand()%5;
		j=rand()%5;
		array_libri[i]++;
		array_libri[j]++;
		sleep(5);
	}

}


void *thread_gestisci_risposta(void *arg){

	long socket = (long)arg;
	fine=1;
	ordine_libri=1;
	   
	    
	

	while(fine)
		    {
			
			pthread_mutex_lock(&ordine);
				
			while(ordine_libri!=0){
				char buffer[BUFSIZE];int n;
				memset(buffer, 0x0, BUFSIZE);
				SYSCALL(n, read(socket, buffer, BUFSIZE), "read");
				
				compra(buffer);

				SYSCALL(n, writen(socket, buffer, sizeof(buffer)), "write");
				
			}
			pthread_mutex_unlock(&ordine);
			pthread_mutex_lock(&pagamento);
			
		    	
			
			int pagato=0;
			
			while(!pagato){
				char buffer[BUFSIZE];
				memset(buffer, 0x0, BUFSIZE);
				int n;
				SYSCALL(n, read(socket, buffer, BUFSIZE), "read");

				pagato=pagamento_(buffer);
				SYSCALL(n, writen(socket, buffer, sizeof(buffer)), "write");
			}
			
			pthread_mutex_unlock(&pagamento);

			
			pthread_cond_broadcast(&aspetto_fine_th);
			fine=0;
			
		}
        	close(sock_file_d);
			
	        return NULL;

}


void *thread_gestisci_client(void *arg)
{
	
	fine=1;
    	long socket = (long)arg;
	
	pthread_t thread;
	
	do{
	   
	    
		char buffer[BUFSIZE];
		memset(buffer, 0x0, BUFSIZE);
		int n;
        	SYSCALL(n, read(socket, buffer, BUFSIZE), "read");
	

		if (n == 0){
		    break;
		}
		if (strncmp(buffer, "Ciao", 4) == 0){
		    
		SYSCALL(n, writen(socket, libri_disponibili, sizeof(libri_disponibili)), "write");

		}else if(strncmp(buffer, "ordine", 6) == 0){ //ricevuto ordine
			
			if(occupato){
				pthread_mutex_lock(&fine_th);
				SYSCALL(n, writen(socket, ordinare, sizeof(ordinare)), "write");
				occupato=0;
				if (pthread_create(&thread, NULL, thread_gestisci_risposta, (void *)(intptr_t)socket) != 0)
				    {
					perror("pthread_create");
				    }

				
				pthread_cond_wait(&aspetto_fine_th,&fine_th);
				
				
				pthread_mutex_unlock(&fine_th);
				occupato=1;
				close(socket);
			}else{
				SYSCALL(n, writen(socket, "OCCUPATO, riprovare piu' tardi", sizeof("OCCUPATO, riprovare piu' tardi")), "write");
			}
		}else{
			SYSCALL(n, writen(socket, "Messaggio errato", sizeof("Messaggio errato")), "write");
		}
		
	}while(fine);
	return NULL;
}


void genera_thread(int sock_file_d)
{
    pthread_attr_t tid;
    pthread_t thid;
    if (pthread_attr_init(&tid) != 0)
    {
        perror("pthread_attr_init");
        close(sock_file_d);
        return;
    }

    if (pthread_attr_setdetachstate(&tid, PTHREAD_CREATE_DETACHED) != 0)
    {
        perror("pthread_attr_setdetachstate");
        pthread_attr_destroy(&tid);
        close(sock_file_d);
        return;
    }
    if (pthread_create(&thid, &tid, thread_gestisci_client, (void *)(intptr_t)sock_file_d) != 0)
    {
        perror("pthread_create");
        pthread_attr_destroy(&tid);
        close(sock_file_d);
        return;
    }
    return ;
}

int main(void)
{
	if (atexit(cleanup) < 0) 
	{
		perror("atexit");
		exit(EXIT_FAILURE);
	}
    pthread_attr_t attr;
    struct sockaddr_un s_addr;
    pthread_t pthre;
    int d_int;
    int listen_d; //file descriptor

//inizializzo gli attributi
    if(pthread_attr_init(&attr)!=0){
	perror("pthread_attr_init");
	return 0;
   	
    }

    if(pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED)!=0){
	perror("pthread_attr_setdetachstate");
	pthread_attr_destroy(&attr);
	return 0;
    }

    if(pthread_create(&pthre,&attr,thread_magazzino,NULL)!=0){
	perror("pthread_create");
	pthread_attr_destroy(&attr);
	return 0;
    }


    SYSCALL(listen_d, socket(AF_UNIX, SOCK_STREAM, 0), "socket"); //AF_UNIX perche' siamo nella stessa macchina
    
//resettare quando creiamo questo tipo di struct per evitare problemi nella memoria
    memset(&s_addr, '0', sizeof(s_addr));
    s_addr.sun_family=AF_UNIX;
    strncpy(s_addr.sun_path, SOCKNAME, strlen(SOCKNAME)+1);
	
// assegno l'indirizzo al socket listen_d con bind
    SYSCALL(d_int, bind(listen_d, (struct sockaddr *)&s_addr, sizeof(s_addr)), "bind");
//mi metto in ascolto di connessioni
    SYSCALL(d_int, listen(listen_d, 1), "listen"); 

//accetto tutte le connessioni
    do
    {
        SYSCALL(sock_file_d, accept(listen_d, (struct sockaddr *)NULL, NULL), "accept"); // bloccarsi in attesa
        genera_thread(sock_file_d);
    } while (1);

    close(listen_d);
    return 0;
}







