/* $begin select */
#include "csapp.h"
void echo(int connfd);
void command(void);

int main(int argc, char **argv) 
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    fd_set read_set, ready_set;

    if (argc != 2) {
	    fprintf(stderr, "usage: %s <port>\n", argv[0]);
	    exit(0);
    }
    listenfd = Open_listenfd(argv[1]);

    FD_ZERO(&read_set);              /* Clear read set */
    FD_SET(STDIN_FILENO, &read_set); /* Add stdin to read set */ 
    FD_SET(listenfd, &read_set);     /* Add listenfd to read set */ 

    while (1) {
        ready_set = read_set;
        Select(listenfd+1, &ready_set, NULL, NULL, NULL);  /*阻塞进程,直到有描述符可读*/
        if (FD_ISSET(STDIN_FILENO, &ready_set))            /*如果可读的是标准输入...*/            
            command(); 
        if (FD_ISSET(listenfd, &ready_set)) {              /*如果可读的是监听描述符...*/
                clientlen = sizeof(struct sockaddr_storage); 
            connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
            echo(connfd); 
            Close(connfd);
        }
    }
}

void command(void) {
    char buf[MAXLINE];
    if (!Fgets(buf, MAXLINE, stdin))
	    exit(0); /* EOF */
    printf("%s", buf); /* Process the input command */
}
/* $end select */


