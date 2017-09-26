include ../Make.defines

PROGS = locknone

all: ${PROGS}

locknone: lockmain.o locknone.o
	${CC} ${CFLAGS} -o $@ lockmain.o locknone.o ${LIBS}

clean:
	rm -f ${PROGS} ${CLEANFILES}
