CC=colorgcc
CFLAGS=-g -ansi -pedantic -Wall

#GTKFLAGS=`pkg-config --cflags --libs gtk+-2.0`
#CFLAGS=-g -ansi -pedantic -Wall -Wextra

all: dazibao

dazibao: dazibao.o dazibao_read.o dazibao_write.o dazibao_utilities.o dazibao_gui.o
	$(CC) $(CFLAGS) dazibao.o dazibao_read.o dazibao_write.o dazibao_utilities.o dazibao_gui.o `pkg-config --cflags --libs gtk+-2.0` -o dazibao

dazibao.o: dazibao.h dazibao.c
	$(CC) $(CFLAGS) -c dazibao.c `pkg-config --cflags --libs gtk+-2.0` -o dazibao.o

dazibao_read.o: dazibao_read.h dazibao_read.c
	$(CC) $(CFLAGS) -c dazibao_read.c -o dazibao_read.o

dazibao_write.o: dazibao_write.h dazibao_write.c
	$(CC) $(CFLAGS) -c dazibao_write.c -o dazibao_write.o

dazibao_utilities.o: dazibao_utilities.h dazibao_utilities.c
	$(CC) $(CFLAGS) -c dazibao_utilities.c -o dazibao_utilities.o

dazibao_gui.o: dazibao_gui.h dazibao_gui.c
	$(CC) $(CFLAGS) -c dazibao_gui.c `pkg-config --cflags --libs gtk+-2.0` -o dazibao_gui.o

clean:
	rm *.o dazibao