#include "unpipc.h"

void client(int, int);
void server(int, int);

int main(int argc, char **argv)
{
    int pipe1[2], pipe2[2];
    pid_t childpid;

    /* create two pipes */
    Pipe(pipe1);
    Pipe(pipe2);

    /* child */
    if(0 == (childpid = Fork()))
    {
        Close(pipe1[1]);
        Close(pipe2[0]);

        server(pipe1[0], pipe2[1]);
        exit(0);
    }

    /* 4parent */
    Close(pipe1[0]);
    Close(pipe2[1]);

    client(pipe2[0], pipe1[1]);

    /* wait for child to terminate */
    Waitpid(childpid, NULL, 0);
    exit(0);
}
