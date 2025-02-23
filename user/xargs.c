#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
    if (argc < 2) {
        fprintf(2, "xargs: No command to execute.");
        exit(1);
    }
    char* cmd;
    cmd = argv[1];

    char* params[MAXARG + 1];
    char buf[MAXPATH];
    int idx = 0;
    int base_args = argc - 1;
    for (int i = 1; i < argc; i++) {
        params[i - 1] = argv[i];
    }

    while (1) {
        if (read(0, &buf[idx], 1) == 0) 
            break; 
        if (buf[idx] == '\n') {
            buf[idx] = 0;
            if (idx > 0) {
                int pid = fork();
                if (pid == 0) {
                    int arg_idx = 0;
                    for (; arg_idx < base_args; arg_idx++) {
                        params[arg_idx] = argv[arg_idx + 1];
                    }
                    params[arg_idx++] = buf;
                    params[arg_idx] = 0;
                    exec(cmd, params);
                } else {
                    wait((int *)0);
                }
                
            } 
            idx = 0;
        } else {
            idx++;
            if (idx >= sizeof(buf) - 1) {
                fprintf (2, "xargs: line too long!\n");
                exit(1);
            }
        }
    }
    exit(0);
}