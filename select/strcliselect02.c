#include "unp.h"

void str_cli(FILE *fp, int sockfd) {
    int stdineof = 0;

    for (;;) {
        fd_set rset;
        FD_ZERO(&rset);
        if (stdineof == 0) {
            FD_SET(fileno(fp), &rset);
        }
        FD_SET(sockfd, &rset);
        int maxfdp1 = max(fileno(fp), sockfd) + 1;
        Select(maxfdp1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rset)) { /* socket is readable */
            int  n = 0;
            char buf[MAXLINE];
            if ((n = Read(sockfd, buf, MAXLINE)) == 0) {
                if (stdineof == 1) {
                    return; /* normal termination */
                }
                err_quit("str_cli: server terminated prematurely");
            }

            Write(fileno(stdout), buf, n);
        }

        if (FD_ISSET(fileno(fp), &rset)) { /* input is readable */
            // finish send data
            int  n = 0;
            char buf[MAXLINE];
            if ((n = Read(fileno(fp), buf, MAXLINE)) == 0) {
                stdineof = 1;
                buf[0] = 'o';
                buf[1] = 'k';
                Writen(sockfd, buf, n);
                FD_CLR(fileno(fp), &rset);
                continue;
            }

            Writen(sockfd, buf, n);
            Shutdown(sockfd, SHUT_WR); /* send FIN */
        }
    }
}
