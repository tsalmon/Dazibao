CC=gcc
CFLAGS=-g -ansi -pedantic -Wall -Wextra -Wno-unused-parameter
GTK=`pkg-config --cflags --libs gtk+-2.0`
SDL=`sdl-config --libs` -lSDLmain -lSDL -lSDL_image 

all: dazibao

dazibao: dazibao.o dazibao_read.o dazibao_write.o dazibao_utilities.o dazibao_compaction.o dazibao_vue.o
	$(CC) $(CFLAGS) dazibao.o  dazibao_read.o dazibao_write.o dazibao_utilities.o dazibao_compaction.o dazibao_vue.o $(GTK) -o dazibao

dazibao_compaction.o: dazibao_compaction.c
	$(CC) $(CFLAGS) -c dazibao_compaction.c -o dazibao_compaction.o

dazibao.o: dazibao.h dazibao.c
	$(CC) $(CFLAGS) -c dazibao.c $(GTK) -o dazibao.o

dazibao_safe.o: dazibao_safe.h dazibao_safe.c
	$(CC) $(CFLAGS) -c dazibao_safe.c -o dazibao_safe.o

dazibao_read.o: dazibao_read.h dazibao_read.c
	$(CC) $(CFLAGS) -c dazibao_read.c -o dazibao_read.o

dazibao_write.o: dazibao_write.h dazibao_write.c
	$(CC) $(CFLAGS) -c dazibao_write.c -o dazibao_write.o

dazibao_utilities.o: dazibao_utilities.h dazibao_utilities.c
	$(CC) $(CFLAGS) -c dazibao_utilities.c -o dazibao_utilities.o

dazibao_vue.o: dazibao_vue.h dazibao_vue.c
	$(CC) $(CFLAGS) -c dazibao_vue.c $(GTK) -o dazibao_vue.o

clean:
	rm *.o dazibao
