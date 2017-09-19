include ../Make.defines

PROGS = mainpipe fduplex mainpopen mainfifo client_fifo server_fifo

all: ${PROGS}

mainpipe: mainpipe.o client.o server.o
	${CC} ${CFLAGS} -o $@ mainpipe.o client.o server.o ${LIBS}

fduplex: fduplex.o
	${CC} ${CFLAGS} -o $@ fduplex.o ${LIBS}

client_fifo: client_main.o client.o
	${CC} ${CFLAGS} -o $@ client_main.o client.o ${LIBS}

server_fifo: server_main.o server.o
	${CC} ${CFLAGS} -o $@ server_main.o server.o ${LIBS}

mainpopen: mainpopen.o
	${CC} ${CFLAGS} -o $@ mainpopen.o ${LIBS}

mainfifo: mainfifo.o client.o server.o
	${CC} ${CFLAGS} -o $@ mainfifo.o client.o server.o ${LIBS}

clean:
	rm -f ${PROGS} ${CLEANFILES}
