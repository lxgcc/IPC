#include "unpipc.h"

#define NBUFF   8

/* data shared by producer and consumer */
struct {
    struct {
        /* a buffer */
        char data[BUFFSIZE];
        /* count of #bytes in the buffer */
        ssize_t n;
    } buff[NBUFF]; /* NBUFF of these buffers/counts */
    sem_t mutex, nempty, nstored;
} shared;

int fd;
void *produce(void *);
void *consume(void *);

int main(int argc, char **argv)
{
    pthread_t tid_produce, tid_consume;

    if(2 != argc)
        err_quit("usage: mycat2 <pathname>");

    fd = Open(argv[1], O_RDONLY);

    /* 4initialize three semaphores */
    Sem_init(&shared.mutex, 0, 1);
    Sem_init(&shared.nempty, 0, NBUFF);
    Sem_init(&shared.nstored, 0, 0);

    /* 4one producer thread, one consumer thread */
    Set_concurrency(2);
    Pthread_create(&tid_produce, NULL, produce, NULL);
    Pthread_create(&tid_consume, NULL, consume, NULL);

    Pthread_join(tid_produce, NULL);
    Pthread_join(tid_consume, NULL);

    Sem_destroy(&shared.mutex);
    Sem_destroy(&shared.nempty);
    Sem_destroy(&shared.nstored);

    exit(0);
}

/* include produce */
void * produce(void *arg)
{
    int i;
    for(i=0; ; ) {
        /* wait for at least 1 empty slot */
        Sem_wait(&shared.nempty);
        Sem_wait(&shared.mutex);
        /* 4critical region */
        Sem_post(&shared.mutex);

        shared.buff[i].n = Read(fd, shared.buff[i].data, BUFFSIZE);
        if(0 == shared.buff[i].n) {
            Sem_post(&shared.nstored);
            return(NULL);
        }
        if(++i >= NBUFF)
            i = 0;

        Sem_post(&shared.nstored);
    }
}

void * consume(void *arg)
{
    int i;
    for(i=0; ; ) {
        Sem_wait(&shared.nstored);
        Sem_wait(&shared.mutex);
        /* critical region */
        Sem_post(&shared.mutex);

        if(0 == shared.buff[i].n)
            return(NULL);
        Write(STDOUT_FILENO, shared.buff[i].data, shared.buff[i].n);
        if(++i >= NBUFF)
            i = 0;

        Sem_post(&shared.nempty);
    }
}


















