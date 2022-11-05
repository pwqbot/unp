/* include fig01 */
#include "unp.h"

int main(int argc, char **argv) {
    struct sockaddr_in servaddr;

    int listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(SERV_PORT);

    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    int maxfd = listenfd; /* initialize */
    int maxi  = -1;       /* index into client[] array */
    int client[FD_SETSIZE];
    for (int i = 0; i < FD_SETSIZE; i++) {
        client[i] = -1; /* -1 indicates available entry */
    }

    fd_set rset, allset;
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    /* end fig01 */

    /* include fig02 */
    for (;;) {
        rset = allset; /* structure assignment */

        int nready = Select(maxfd + 1, &rset, NULL, NULL, NULL);
        if (FD_ISSET(listenfd, &rset)) { /* new client connection */
            struct sockaddr_in cliaddr;
            socklen_t          clilen = sizeof(cliaddr);
            int connfd = Accept(listenfd, (SA *)&cliaddr, &clilen);
#ifdef NOTDEF
            printf("new client: %s, port %d\n",
                   Inet_ntop(AF_INET, &cliaddr.sin_addr, 4, NULL),
                   ntohs(cliaddr.sin_port));
#endif

            // find first usable position
            for (int i = 0; i <= FD_SETSIZE; i++) {
                if (client[i] < 0) {
                    client[i] = connfd; /* save descriptor */
                    break;
                }
                if (i == FD_SETSIZE) {
                    err_quit("too many clients");
                }
                if (i > maxi) {
                    maxi = i; /* max index in client[] array */
                }
            }

            FD_SET(connfd, &allset); /* add new descriptor to set */
            if (connfd > maxfd) {
                maxfd = connfd; /* for select */
            }

            if (--nready <= 0) {
                continue; /* no more readable descriptors */
            }
        }

        for (int i = 0; i <= maxi; i++) { /* check all clients for data */
            const int sockfd = client[i];
            if (sockfd < 0) {
                continue;
            }

            if (FD_ISSET(sockfd, &rset)) {
                char    buf[MAXLINE];
                ssize_t n = Read(sockfd, buf, MAXLINE);
                if (n == 0) {
                    /*4connection closed by client */
                    Close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                } else {
                    Writen(sockfd, buf, n);
                }

                if (--nready <= 0) {
                    break; /* no more readable descriptors */
                }
            }
        }
    }
}
/* end fig02 */
