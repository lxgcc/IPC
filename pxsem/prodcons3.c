#include "unpipc.h"

#define NBUFF 10
#define MAXNTHREADS 100


/* read-only by producer and consumer */
int nitems;
int nproducers;

/* data shared by producer adn consumer */
struct {
    int buff[NBUFF];
    int nput;
    int nputval;
    sem_t mutex, nempty, nstored;
} shared;

void *produce(void *);
void *consume(void *);

int main(int argc, char **argv)
{
    int i;
    int count[MAXNTHREADS];
    pthread_t tid_produce[MAXNTHREADS], tid_consume;

    if(3 != argc)
        err_quit("usage: prodcons1 <#items> <#producers>");
    nitems = atoi(argv[1]);
    nproducers = min(atoi(argv[2]), MAXNTHREADS);

    /* initialize three semaphores */
    Sem_init(&shared.mutex, 0, 1);
    Sem_init(&shared.nempty, 0, NBUFF);
    Sem_init(&shared.nstored, 0, 0);

    /* 4create one producer thread and one consumer thread */
    Set_concurrency(nproducers + 1);
    for(i = 0; i < nproducers; ++i)
    {
        count[i] = 0;
        Pthread_create(&tid_produce[i], NULL, produce, &count[i]);
    }
    Pthread_create(&tid_consume, NULL, consume, NULL);

    printf("for debug\n");
    /* wait for all producers and the consumer */
    for(i=0; i<nproducers; ++i)
    {
        Pthread_join(tid_produce[i], NULL);
        printf("count[%d] = %d\n", i, count[i]);
    }
    Pthread_join(tid_consume, NULL);

    printf("for debug\n");

    Sem_destroy(&shared.mutex);
    Sem_destroy(&shared.nempty);
    Sem_destroy(&shared.nstored);
    exit(0);
}

/* include prodcons */
void *produce(void *arg)
{
    for( ; ; ) {
        /* wait for at least 1 empty slot */
        Sem_wait(&shared.nempty);
        Sem_wait(&shared.mutex);

        if(shared.nput >= nitems) {
            Sem_post(&shared.nempty);
            Sem_post(&shared.mutex);
            return(NULL);
        }

        shared.buff[shared.nput % NBUFF] = shared.nputval;
        shared.nput++;
        shared.nputval++;
        
        Sem_post(&shared.mutex);
        Sem_post(&shared.nstored);
        *((int *) arg) += 1;
    }
}

void *consume(void *arg)
{
    int i;
    for(i=0; i<nitems; ++i)
    {
        /* wait for at least 1 stored item */
        Sem_wait(&shared.nstored);
        Sem_wait(&shared.mutex);

        if(i != shared.buff[i % NBUFF])
            printf("error: buff[%d] = %d\n", i, shared.buff[i % NBUFF]);

        Sem_post(&shared.mutex);
        Sem_post(&shared.nempty);
    }
    return (NULL);
}
