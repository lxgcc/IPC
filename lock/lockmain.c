#include "unpipc.h"

/* filename */
#define SEQFILE "seqno"

int main(int argc, char **argv)
{
    int fd;
    long i;
    long seqno;

    pid_t pid;
    ssize_t n;
    char line[MAXLINE+1];

    pid = getpid();
    fd = Open(SEQFILE, O_RDWR, FILE_MODE);

    for(i=0; i<20; ++i)
    {
        my_lock(fd);

        /* rewind before read */
        Lseek(fd, 0L, SEEK_SET);
        n = Read(fd, line, MAXLINE);
        line[n] = '\0';

        n = sscanf(line, "%ld\n", &seqno);
        printf("%s: pid = %ld, seq# = %ld\n", argv[0], (long)pid, seqno);

        /* increment sequence number */
        seqno++;

        snprintf(line, sizeof(line), "%ld\n", seqno);
        Lseek(fd, 0L, SEEK_SET);
        Write(fd, line, strlen(line));

        my_unlock(fd);
    }
    exit(0);
}
