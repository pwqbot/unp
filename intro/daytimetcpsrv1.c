#include "unp.h"
#include <time.h>

int main(int argc, char **argv) {
    int listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(13); /* daytime server */

    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    socklen_t          len = sizeof(servaddr);
    getsockname(listenfd, (SA *)&servaddr, &len);
    printf("connection local ip is %s, local port is %d\n",
           sock_ntop((SA *)&servaddr, len), ntohs(servaddr.sin_port));

    Listen(listenfd, LISTENQ);

    for (;;) {
        struct sockaddr_in cliaddr;
        socklen_t          len    = sizeof(cliaddr);
        int                connfd = Accept(listenfd, (SA *)&cliaddr, &len);

        char buff[MAXLINE];
        printf("connection from %s, port %d\n",
               Inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
               ntohs(cliaddr.sin_port));

        time_t ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buff, strlen(buff));

        Close(connfd);
    }
}
