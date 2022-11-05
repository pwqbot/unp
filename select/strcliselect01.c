#include "unp.h"
#include <stdio.h>

void str_cli(FILE *fp, int sockfd) {
    for (;;) {
        fd_set rset;
        FD_ZERO(&rset);
        FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);

        char sendline[MAXLINE], recvline[MAXLINE];
        int  maxfdp1 = max(fileno(fp), sockfd) + 1;
        Select(maxfdp1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rset)) { /* socket is readable */
            if (Readline(sockfd, recvline, MAXLINE) == 0) {
                err_quit("str_cli: server terminated prematurely");
            }
            Fputs(recvline, stdout);
        }

        if (FD_ISSET(fileno(fp), &rset)) { /* input is readable */
            if (Fgets(sendline, MAXLINE, fp) == NULL) {
                return; /* all done */
            }
            Writen(sockfd, sendline, strlen(sendline));
        }
    }
}
