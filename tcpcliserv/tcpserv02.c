#include "unp.h"

void sig_chld(int);

int main(int argc, char **argv) {
    struct sockaddr_in servaddr;

    int listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(SERV_PORT);

    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    Signal(SIGCHLD, sig_chld);

    for (;;) {
        struct sockaddr_in cliaddr;
        socklen_t          clilen = sizeof(cliaddr);
        int                connfd = Accept(listenfd, (SA *)&cliaddr, &clilen);

        pid_t childpid = Fork();
        if (childpid == 0) {  /* child process */
            Close(listenfd);  /* close listening socket */
            str_echo(connfd); /* process the request */
            exit(0);
        }
        Close(connfd); /* parent closes connected socket */
    }
}
