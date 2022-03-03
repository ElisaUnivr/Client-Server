#if !defined(UTIL_H)
#define UTIL_H


#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h> 

#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define SOCKNAME "./socket_esame"

#define BUFSIZE 400
#define UNIX_PATH_MAX 108

#define SYSCALL(r,c,e) 	\
	if((r=c)==-1) 	\
	{		\
	   perror(e);	\
	   exit(errno);	\
	}



pthread_mutex_t ordine=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t pagamento=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fine_th=PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t aspetto_fine_th= PTHREAD_COND_INITIALIZER;

static inline int writen(long fd, void *buf, size_t size){
	size_t left=size;
	int r;
	char *bufptr=(char *) buf;
	while(left>0){
		if((r=write((int)fd,bufptr,left))==-1){
			if(errno==EINTR){ continue;}
			return -1;


}
		if(r==0){ return 0;}
		left-=r;
		bufptr+=r;



}
	return 1;

}









#endif /* UTIL_H */
