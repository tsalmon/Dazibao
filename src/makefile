LDFLAGS=-Wall `pkg-config --cflags --libs gtk+-2.0` `sdl-config --libs` -lSDLmain -lSDL -lSDL_image 
CC=gcc
FILE=vue

dazibao: $(FILE).o test.o
	$(CC) -g -o $(FILE) test.o $(FILE).o $(LDFLAGS)

test.o: test.c $(FILE).h
	$(CC) -g -o test.o -c test.c   -c $(LDFLAGS)

$(FILE).o: $(FILE).c
	$(CC) -g -o $(FILE).o -c $(FILE).c $(LDFLAGS)

clean:
	rm *.o