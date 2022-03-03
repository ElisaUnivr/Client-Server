#include "util.h"


void read_line(char *buf)
{
    printf("> ");
    fflush(stdout);
    memset(buf, '\0', BUFSIZE);
    if (read(STDIN_FILENO, buf, BUFSIZE) == -1)
    {
        perror("leggendo dallo standard input:");
        exit(EXIT_FAILURE);
    }
}



int main(void)
{
    struct sockaddr_un s_addr;
    int sock_d; //file descriptor
    SYSCALL(sock_d, socket(AF_UNIX, SOCK_STREAM, 0), "socket");
    memset(&s_addr, '0', sizeof(s_addr));

//per definire i campi della struct s_addr
    s_addr.sun_family = AF_UNIX;
    strncpy(s_addr.sun_path, SOCKNAME, strlen(SOCKNAME) + 1);

    int d_int, n;
    SYSCALL(d_int, connect(sock_d, (struct sockaddr *)&s_addr, sizeof(s_addr)), "connect");
    
    char buf[BUFSIZE];
    do
    {//se il client mi invia quit termino
        read_line(buf);
        if (strncmp(buf, "quit", 4) == 0){
            break;
	}
//invio il messaggio al server
   	SYSCALL(d_int, writen(sock_d, buf, strlen(buf)), "writen");
//rispost del server
        SYSCALL(n, read(sock_d, buf, BUFSIZE), "read"); 
        buf[n] = '\0';// non so quanto e' grande a priori
        printf("%s\n", buf);


    }while(1);
    close(sock_d);

    return 0;


}

