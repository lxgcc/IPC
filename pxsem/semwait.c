#include "unpipc.h"

int main(int argc, char **argv)
{
    sem_t *sem;
    int val;

    if(2 != argc)
        err_quit("usage: semwait <name>");
    sem = Sem_open(argv[1], 0);
    Sem_wait(sem);
    Sem_getvalue(sem, &val);
    printf("pid %ld has semaphore, value = %d\n", (long) getpid(), val);

    pause();
    exit(0);
}
            
