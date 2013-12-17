CC=gcc
CFLAGS=-g -ansi -pedantic -Wall
#CFLAGS=-g -ansi -pedantic -Wall -Wextra
all: dazibao

dazibao: dazibao.o dazibao_read.o dazibao_write.o
	$(CC) $(CFLAGS) dazibao.o dazibao_read.o dazibao_write.o -o dazibao

dazibao.o: dazibao.h dazibao.c
	$(CC) $(CFLAGS) -c dazibao.c -o dazibao.o

dazibao_read.o: dazibao_read.h dazibao_read.c
	$(CC) $(CFLAGS) -c dazibao_read.c -o dazibao_read.o

dazibao_write.o: dazibao_write.h dazibao_write.c
	$(CC) $(CFLAGS) -c dazibao_write.c -o dazibao_write.o

clean:
	rm *.o dazibao
