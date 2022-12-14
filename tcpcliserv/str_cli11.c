#include "unp.h"
#include <asm-generic/errno.h>

void str_cli(FILE *fp, int sockfd) {
    char sendline[MAXLINE], recvline[MAXLINE];

    while (Fgets(sendline, MAXLINE, fp) != NULL) {

        Writen(sockfd, sendline, 1); // after this action, send rst
        sleep(1);
        Writen(sockfd, sendline + 1, strlen(sendline) - 1);

        if (Readline(sockfd, recvline, MAXLINE) == 0) {
            err_quit("str_cli: server terminated prematurely");
        }

        Fputs(recvline, stdout);
    }
}
