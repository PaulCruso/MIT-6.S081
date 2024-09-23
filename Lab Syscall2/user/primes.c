#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h" 

void prime(int *p1) {
    int n;
    int p2[2];
    close(p1[1]);
    if (read(p1[0], &n, sizeof(int)) != sizeof(int)) {
        close(p1[0]);
        exit(0);
    }
    pipe(p2);

    if (fork() == 0) {
        prime(p2);
    } else {
        close(p2[0]);
        printf("prime %d\n", n);
        int currPrime = n;
        while (read(p1[0], &n, sizeof(int)) == sizeof(int)) {
            if (n % currPrime != 0) {
                write(p2[1], &n, sizeof(int));
            }
        }
        close(p1[0]);
        close(p2[1]);
        wait(0);
        exit(0);
    }
}


int main(int argc, char* argv[]) {
    int p1[2];
    if (pipe(p1) < 0) {
        exit(1);
    }

    if (fork() == 0) {
        prime(p1);
    } else {
        close(p1[0]);
        for (int i = 2; i <= 35; i++) {
            int n = i;
            write(p1[1], &n, sizeof(int));
        }
        close(p1[1]);
        wait(0);
    }

    exit(1);
}