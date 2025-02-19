#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    char buf[1];
    int p1[2], p2[2];

    pipe(p1);
    pipe(p2);

    int pid = fork();
    if (pid < 0) {
        fprintf(2, "Error: fork failed.\n");
        exit(1);
    }
    if (pid == 0) { // Child Process
        close(p1[1]);
        close(p2[0]);
        read(p1[0], buf, 1);
        fprintf(1, "%d: received ping\n", getpid());
        write(p2[1], buf, 1);
        close(p1[0]);
        close(p2[1]);
        exit(0);
    } else { // Parent Process
        close(p1[0]);
        close(p2[1]);
        write(p1[1], " ", 1);
        read(p2[0], buf, 1);
        fprintf(1, "%d: received pong\n", getpid());
        close(p1[1]);
        close(p2[0]);
        exit(0);
    }
}