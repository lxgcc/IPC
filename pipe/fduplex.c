#include "unpipc.h"

int main(int argc, char **argv)
{
    int fd[2], n;
    char c;
    pid_t childpid;

    /* assumes a full-duplex pipe */
    Pipe(fd);
    if(0 == (childpid = Fork())){
        sleep(3);
        if(1 != (n = Read(fd[0], &c, 1)))
            err_quit("child: read returned %d", n);
        printf("child read %c\n", c);
        Write(fd[0], "c", 1);
        exit(0);
    }

    /* 4parent */
    Write(fd[1], "p", 1);
    if(1 != (n = Read(fd[1], &c, 1)))
        err_quit("parent: read returned %d", n);
    printf("parent read %c\n", c);
    exit(0);
}
