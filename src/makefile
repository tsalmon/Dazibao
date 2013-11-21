LDFLAGS=-Wall `pkg-config --cflags --libs gtk+-2.0` `sdl-config --libs` -lSDLmain -lSDL -lSDL_image 
CC=gcc
FILE=vue

main: $(FILE).o	test.o
	$(CC) -g  -o $(FILE) test.o $(FILE).o $(LDFLAGS)

test.o: test.c
	$(CC) -g test.c -c $(LDFLAGS)

$(FILE).o: $(FILE).c
	$(CC) -g $(FILE).c -c $(LDFLAGS)

clean:
	rm *.o