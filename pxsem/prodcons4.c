#include "unpipc.h"

#define NBUFF       10
#define MAXNTHREADS 100

/* read-only */
int nitems;
int nproducers;
int nconsumers;

/* data shared by by producers and consumers */
struct {
    int buff[NBUFF];
    int nput;
    int nputval;
    int nget;
    int ngetval;
    sem_t mutex, nempty, nstored;
} shared;

void *produce(void *);
void *consume(void *);

int main(int argc, char **argv)
{
    int i;
    int prodcount[MAXNTHREADS];
    int conscount[MAXNTHREADS];

    pthread_t tid_produce[MAXNTHREADS];
    pthread_t tid_consume[MAXNTHREADS];

    if(4 != argc)
        err_quit("usage: prodcons4 <#items> <#producers> <#consumers>");
    nitems = atoi(argv[1]);
    nproducers = min(atoi(argv[2]), MAXNTHREADS);
    nconsumers = min(atoi(argv[3]), MAXNTHREADS);

    /* 4initialize three semaphores */
    Sem_init(&shared.mutex, 0, 1);
    Sem_init(&shared.nempty, 0, NBUFF);
    Sem_init(&shared.nstored, 0, 0);

    /* 4create all producers and all consumers */
    Set_concurrency(nproducers + nconsumers);
    for(i=0; i<nproducers; ++i){
        prodcount[i] = 0;
        Pthread_create(&tid_produce[i], NULL, produce, &prodcount[i]);
    }
    for(i=0; i<nconsumers; ++i){
        conscount[i] = 0;
        Pthread_create(&tid_consume[i], NULL, consume, &conscount[i]);
    }

    /* 4wait for all producers and all consumers */
    for(i=0; i<nproducers; ++i) {
        Pthread_join(tid_produce[i], NULL);
        printf("producer count[%d] = %d\n", i, prodcount[i]);
    }
    for(i=0; i<nconsumers; ++i) {
        Pthread_join(tid_consume[i], NULL);
        printf("consumer count[%d] = %d\n", i, conscount[i]);
    }

    Sem_destroy(&shared.mutex);
    Sem_destroy(&shared.nempty);
    Sem_destroy(&shared.nstored);
    exit(0);
}

/* include produce */
void* produce(void *arg)
{
    for( ; ; ) {
        /* wait for at least 1 empty slot */
        Sem_wait(&shared.nempty);
        Sem_wait(&shared.mutex);

        if(shared.nput >= nitems) {
            /* let consumers terminate */
            Sem_post(&shared.nstored);
            Sem_post(&shared.nempty);
            Sem_post(&shared.mutex);
            return(NULL);
        }

        shared.buff[shared.nput % NBUFF] = shared.nputval;
        ++shared.nput;
        ++shared.nputval;

        Sem_post(&shared.mutex);
        Sem_post(&shared.nstored);
        *((int *) arg) += 1;
    }
}

void* consume(void *arg)
{
    int i;
    for( ; ; ) {
        /* wait for at least 1 stored item */
        Sem_wait(&shared.nstored);
        Sem_wait(&shared.mutex);

        if(nitems <= shared.nget) {
            Sem_post(&shared.nstored);
            Sem_post(&shared.mutex);
            return(NULL);
        }

        i = shared.nget % NBUFF;
        if(shared.buff[i] != shared.ngetval)
            printf("error: buff[%d] = %d\n", i, shared.buff[i]);
        ++shared.nget;
        ++shared.ngetval;

        Sem_post(&shared.mutex);
        Sem_post(&shared.nempty);
        *((int *) arg) += 1;
    }
}



