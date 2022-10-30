#include "unp.h"

void sig_chld(int signo) {
    int   stat = 0;
    pid_t pid  = wait(&stat);
    printf("child %d terminated\n", pid);
}
