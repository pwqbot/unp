#include "unp.h"
#include <netinet/in.h>

struct kk {
    unsigned char a : 1;
    unsigned char b : 8;
    unsigned char c : 8;
    unsigned char d : 8;
};

int main(int argc, char **argv) {
    union {
        short s;
        char  c[sizeof(short)];
    } un;

    un.s = 0x0102;
    struct kk     stdData;

    unsigned int x = 1;
    memcpy(&stdData, &x, sizeof(stdData));
    printf("%d, %d, %d , %d\n", stdData.a, stdData.b, stdData.c, stdData.d);

    x = 30;
    memcpy(&stdData, &x, sizeof(stdData));
    printf("%d, %d, %d , %d\n", stdData.a, stdData.b, stdData.c, stdData.d);
    printf("%d -> %d, %d -> %d\n",1, htons(1), 128, htons(128));

    printf("%s: ", CPU_VENDOR_OS);
    if (sizeof(short) == 2) {
        if (un.c[0] == 1 && un.c[1] == 2) {
            printf("big-endian\n");
        } else if (un.c[0] == 2 && un.c[1] == 1) {
            printf("little-endian\n");
        } else {
            printf("unknown\n");
        }
    } else {
        printf("sizeof(short) = %lu\n", sizeof(short));
    }

    exit(0);
}
