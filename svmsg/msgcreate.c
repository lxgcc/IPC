#include "unpipc.h"

int main(int argc, char **argv)
{
    int c, oflag, mqid;

    oflag = SVMSG_MODE | IPC_CREAT;
    while(-1 != (c = Getopt(argc, argv, "e")))
    {
        switch(c) {
            case 'e':
                oflag |= IPC_EXCL;
                break;
        }
    }
    if(optind != argc - 1)
        err_quit("usage: msgcreate [ -e ] <pathname>");

    mqid = Msgget(Ftok(argv[optind], 0), oflag);
    exit(0);
}


