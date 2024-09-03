#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h" 

int main(int argc, char* argv[]) {
    int p1[2], p2[2];
    if (pipe(p1) < 0) {
        exit(1);
    }

    if (pipe(p2) < 0 ) {
        exit(1);
    }

    if (fork() != 0) {
        char buf;
        write(p1[1], "!", 1);
        close(p1[1]);
        wait(0);
        read(p2[0], &buf, 1);
        close(p2[0]);

        printf("%d: received pong\n", getpid());
    } else {
        char buf;
        read(p1[0], &buf, 1);
        close(p1[0]);
        printf("%d: received ping\n", getpid());
        write(p2[1], "!", 1);
        close(p1[1]);

        exit(0);
    }

    exit(0);
}