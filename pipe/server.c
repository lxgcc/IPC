#include "unpipc.h"

void server(int readfd, int writefd)
{
    int fd;
    ssize_t n;
    char buff[MAXLINE+1];

    /* 4read pathname from IPC channel */
    if(0 == (n = Read(readfd, buff, MAXLINE)))
        err_quit("end-of-file while reading pathname");
    /* null terminate pathname */
    buff[n] = '\0';

    if(0 > (fd = open(buff, O_RDONLY))){
        /* 4error: must tell client */
        snprintf(buff + n, sizeof(buff) - n, ": can't open, %s\n",
                strerror(errno));
        n = strlen(buff);
        Write(writefd, buff, n);
    }
    else {
        /* 4open succeeded: copy file to IPC channel */
        while(0 < (n = Read(fd, buff, MAXLINE)))
            Write(writefd, buff, n);
        Close(fd);
    }
}
