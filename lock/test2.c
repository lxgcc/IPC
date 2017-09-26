#include "unpipc.h"

int main(int argc, char **argv)
{
    int fd;

    fd = Open("test1.data", O_RDWR | O_CREAT, (mode_t) FILE_MODE);

    /* parents read locks entire file */
    Read_lock(fd, 0, SEEK_SET, 0);
    printf("%s: parent has read lock\n", Gf_time());

    /* 4frist child */
    if(0 == Fork()) {
        sleep(1);
        printf("%s: first child tries to obtain write lock\n", Gf_time());
        /* this should block */
        Writew_lock(fd, 0, SEEK_SET, 0);
        printf("%s: first child obtains write lock\n", Gf_time());
        sleep(2);
        Un_lock(fd, 0, SEEK_SET, 0);
        printf("%s: first child releases write lock\n", Gf_time());
        exit(0);
    }

    /* 4second child */
    if(0 == Fork()) {
        sleep(3);
        printf("%s: second child tries to obtain read lock\n", Gf_time());
        Readw_lock(fd, 0, SEEK_SET, 0);
        printf("%s: second child obtains read lock\n", Gf_time());
        sleep(4);
        Un_lock(fd, 0, SEEK_SET, 0);
        printf("%s: second child releases read lock\n", Gf_time());
        exit(0);
    }

    /* 4 parents */
    sleep(5);
    Un_lock(fd, 0, SEEK_SET, 0);
    printf("%s: parents releases read lock\n", Gf_time());
    exit(0);
}

