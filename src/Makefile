CC=gcc
CFLAGS=-g -ansi -pedantic -Wall

GTK=`pkg-config --cflags --libs gtk+-2.0`
SDL=`sdl-config --libs` -lSDLmain -lSDL -lSDL_image 
all: dazibao

dazibao: dazibao.o dazibao_read.o dazibao_write.o dazibao_utilities.o vue.o
	$(CC) $(CFLAGS) dazibao.o dazibao_read.o dazibao_write.o dazibao_utilities.o vue.o $(GTK) $(SDL) -o dazibao

dazibao.o: dazibao.h dazibao.c
	$(CC) $(CFLAGS) -c dazibao.c `pkg-config --cflags --libs gtk+-2.0` -o dazibao.o

dazibao_read.o: dazibao_read.h dazibao_read.c
	$(CC) $(CFLAGS) -c dazibao_read.c -o dazibao_read.o

dazibao_write.o: dazibao_write.h dazibao_write.c
	$(CC) $(CFLAGS) -c dazibao_write.c -o dazibao_write.o

dazibao_utilities.o: dazibao_utilities.h dazibao_utilities.c
	$(CC) $(CFLAGS) -c dazibao_utilities.c -o dazibao_utilities.o

vue.o: vue.h vue.c
	$(CC) $(CFLAGS) -c vue.c $(GTK) $(SDL) -o vue.o

clean:
	rm *.o dazibao