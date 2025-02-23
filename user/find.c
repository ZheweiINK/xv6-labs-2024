#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void
find(char *path, char* filename)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        if (de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if (stat(buf, &st) < 0) {
            printf("find: cannot stat %s\n", buf);
        }

        switch(st.type) {
            case T_DEVICE:
            case T_FILE:
                if (strcmp(de.name, filename) == 0)
                    printf("%s\n", buf);
                break;
            case T_DIR:
                if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                    continue;
                find(buf, filename);
        }
    }
}

int
main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(2, "Error: Please specify directory and file names.\n");
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}