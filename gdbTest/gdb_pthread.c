#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

// Parent process handle
void Parent();
// Child process handle
void Child();
// Parent process handle after generate a thread
void * ParentDo(char *argv);

int main(int argc, const char **argv)
{
    int pid;
    pid = fork();
    // add the first breakpoint
    if(0 != pid)
        Parent();
    else
        Child();
    return 0;
}

// Parent process handle
void Parent()
{
    pid_t pid = getpid();
    char cParent[] = "Parent";
    char cThread[] = "Thread";
    pthread_t pt;

    printf("[%s]: [%d] [%s]\n", cParent, pid, "step1");
    if( 0 != pthread_create( &pt, NULL, (void *)*ParentDo, cThread))
        printf("[%s]: Can not create a thread.\n", cParent);

    ParentDo( cParent );
    sleep(1);
}

void * ParentDo( char *argv )
{
    pid_t pid = getpid();
    // get the thread-id selfly
    pthread_t tid = pthread_self();
    char tprefix[] = "thread";

    printf("[%s]: [%d] [%s] [%lu] [%s]\n", argv, pid, tprefix, tid, "step2");
    printf("[%s]: [%d] [%s] [%lu] [%s]\n", argv, pid, tprefix, tid, "step3");

    return NULL;
}

void Child()
{
    pid_t pid = getpid();
    char prefix[] = "Child";
    printf("[%s]: [%d] [%s]\n", prefix, pid, "step1");
    return;
}



