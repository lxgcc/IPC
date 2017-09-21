include ../Make.defines

PROGS = ctl msgcreate msgsnd msgrcv

all: ${PROGS}

ctl: ctl.o
	${CC} ${CFLAGS} -o $@ ctl.o ${LIBS}

msgcreate: msgcreate.o
	${CC} ${CFLAGS} -o $@ msgcreate.o ${LIBS}

msgsnd: msgsnd.o
	${CC} ${CFLAGS} -o $@ msgsnd.o ${LIBS}

msgrcv: msgrcv.o
	${CC} ${CFLAGS} -o $@ msgrcv.o ${LIBS}

clean:
	rm -f ${PROGS} ${CLEANFILES}

