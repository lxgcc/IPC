include ../Make.defines

PROGS = mainpipe

all: ${PROGS}

mainpipe: mainpipe.o client.o server.o
	${CC} ${CFLAGS} -o $@ mainpipe.o client.o server.o ${LIBS}

clean:
	rm -f ${PROGS} ${CLEANFILES}
