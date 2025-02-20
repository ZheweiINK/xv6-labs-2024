#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void recursive(int pipefd) __attribute__((noreturn));

void
recursive(int pipefd)
{
    int num;
    int p[2];
    if (read(pipefd, &num, sizeof(int)) == 0) {
        close(pipefd);
        exit(0);
    }

    fprintf(1, "prime %d\n", num);

    pipe(p);
    int pid = fork();

    if (pid < 0) {
        fprintf(2, "Error: fork failed.\n");
        exit(1);
    } else if (pid == 0) { // Child Process
        close(p[1]);
        close(pipefd);
        recursive(p[0]);
        exit(0);
    } else { // Parent Process
        close(p[0]);
        int num2;
        while (read(pipefd, &num2, sizeof(int)) != 0) {
            if (num2 % num != 0) {
                write(p[1], &num2, sizeof(int));
            }
        }
        close(pipefd);
        close(p[1]);
        wait((int *) 0);
        exit(0);
    }
}

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    int pid = fork();
    if (pid < 0) {
        fprintf(2, "Error: fork failed.\n");
        exit(1);
    } else if (pid == 0) { // Child Process
        close(p[1]);
        recursive(p[0]);
        exit(0);
    } else { // Parent Process
        close(p[0]);
        for (int i = 2; i <= 280; i++) {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait((int *) 0);
        exit(0);
    }
}