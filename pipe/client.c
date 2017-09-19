#include "unpipc.h"

void client(int readfd, int writefd)
{
    size_t len;
    ssize_t n;
    char buff[MAXLINE];

    /* 4read pathname */
    Fgets(buff, MAXLINE, stdin);
    len = strlen(buff);
    /* delete new line from fgets() */
    if('\n' == buff[len-1])
        len--;

    /* 4write pathname to IPC channel */
    Write(writefd, buff, len);

    /* 4read from IPC, write to standard output */
    while(0 < (n = Read(readfd, buff, MAXLINE)))
        Write(STDOUT_FILENO, buff, n);
}
