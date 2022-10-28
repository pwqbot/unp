#include "unp.h"
#include <time.h>

int main(int argc, char **argv) {
    int                listenfd = 0, connfd = 0;
    struct sockaddr_in servaddr;
    char               buff[MAXLINE];
    time_t             ticks = 0;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(9999); /* daytime server */

    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

    Listen(listenfd, 1);

    for (;;) {
        connfd = Accept(listenfd, (SA *)NULL, NULL);

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        for (size_t i = 0; i < strlen(buff); i++) {
            Write(connfd, buff + i, 1);
        }

        Close(connfd);
    }
}
