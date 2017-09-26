include ../Make.defines

PROGS = locknone lockfcntl test2

all: ${PROGS}

locknone: lockmain.o locknone.o
	${CC} ${CFLAGS} -o $@ lockmain.o locknone.o ${LIBS}

lockfcntl: lockmain.o lockfcntl.o
	${CC} ${CFLAGS} -o $@ lockmain.o lockfcntl.o ${LIBS}

test2: test2.o
	${CC} ${CFLAGS} -o $@ test2.o ${LIBS}

clean:
	rm -f ${PROGS} ${CLEANFILES}
