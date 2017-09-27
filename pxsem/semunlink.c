#include "unpipc.h"

int main(int argc, char **argv)
{
    if(2 != argc)
        err_quit("usage: semunlink <name>");

    Sem_unlink(argv[1]);
    exit(0);
}
