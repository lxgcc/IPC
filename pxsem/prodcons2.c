#include "unpipc.h"

#define NBUFF 10
#define SEM_MUTEX "mutex"
#define SEM_NEMPTY "nempty"
#define SEM_NSTORED "nstored"

/* read-only by producer and consumer */
int nitems;
/* data shared by producer adn consumer */
struct {
    int buff[NBUFF];
    sem_t mutex, nempty, nstored;
} shared;

void *produce(void *);
void *consume(void *);

int main(int argc, char **argv)
{
    pthread_t tid_produce, tid_consume;

    if(2 != argc)
        err_quit("usage: prodcons1 <#items>");
    nitems = atoi(argv[1]);

    /* initialize three semaphores */
    Sem_init(&shared.mutex, 0, 1);
    Sem_init(&shared.nempty, 0, NBUFF);
    Sem_init(&shared.nstored, 0, 0);

    /* 4create one producer thread and one consumer thread */
    Set_concurrency(2);
    Pthread_create(&tid_produce, NULL, produce, NULL);
    Pthread_create(&tid_consume, NULL, consume, NULL);

    /* 4wait for the two threads */
    Pthread_join(tid_produce, NULL);
    Pthread_join(tid_consume, NULL);

    Sem_destroy(&shared.mutex);
    Sem_destroy(&shared.nempty);
    Sem_destroy(&shared.nstored);
    exit(0);
}

/* include prodcons */
void *produce(void *arg)
{
    int i;
    for(i=0; i<nitems; ++i) {
        /* wait for at least 1 empty slot */
        Sem_wait(&shared.nempty);
        Sem_wait(&shared.mutex);
        /* store i into circular buffer */
        shared.buff[i % NBUFF] = i;
        Sem_post(&shared.mutex);
        Sem_post(&shared.nstored);
    }
    return (NULL);
}

void *consume(void *arg)
{
    int i;
    for(i=0; i<nitems; ++i)
    {
        Sem_wait(&shared.nstored);
        Sem_wait(&shared.mutex);
        if(shared.buff[i % NBUFF] != i)
            printf("buff[%d] = %d\n", i, shared.buff[i % NBUFF]);
        Sem_post(&shared.mutex);
        Sem_post(&shared.nempty);
    }
    return (NULL);
}

