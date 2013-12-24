CC=gcc
GTK=`pkg-config --cflags --libs gtk+-2.0` `sdl-config --libs`
SDL=-lSDLmain -lSDL -lSDL_image  
CFLAGS=-g -ansi -pedantic -Wall
#CFLAGS=-g -ansi -pedantic -Wall -Wextra
all: dazibao

dazibao: dazibao.o dazibao_read.o dazibao_write.o vue.o
	$(CC) $(CFLAGS) dazibao.o dazibao_read.o dazibao_write.o vue.o -o dazibao $(GTK) $(SDL)

dazibao.o: dazibao.h dazibao.c vue.h
	$(CC) $(CFLAGS) -c dazibao.c -o dazibao.o $(GTK)

dazibao_read.o: dazibao_read.h dazibao_read.c
	$(CC) $(CFLAGS) -c dazibao_read.c -o dazibao_read.o

dazibao_write.o: dazibao_write.h dazibao_write.c
	$(CC) $(CFLAGS) -c dazibao_write.c -o dazibao_write.o

vue.o: vue.c vue.h
	$(CC) $(CFLAGS) -c vue.c -o vue.o $(GTK) $(SDL)

clean:
	rm *.o dazibao
