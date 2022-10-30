#include "unp.h"

void str_echo(int sockfd) {
    for (;;) {
        ssize_t n    = 0;
        long    arg1 = 0, arg2 = 0;
        char    line[MAXLINE];
        if ((n = Readline(sockfd, line, MAXLINE)) == 0) {
            return; /* connection closed by other end */
        }

        if (sscanf(line, "%ld%ld", &arg1, &arg2) == 2) {
            snprintf(line, sizeof(line), "%ld\n", arg1 + arg2);
        } else {
            snprintf(line, sizeof(line), "input error\n");
        }

        n = strlen(line);
        Writen(sockfd, line, n);
    }
}
