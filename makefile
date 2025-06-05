CC = gcc
CFLAGS = -I/opt/homebrew/include -I/opt/homebrew/include/SDL2
LDFLAGS = -L/opt/homebrew/lib -lSDL2
all:
	$(CC) window.c -o window $(CFLAGS) $(LDFLAGS)
clean: 
	rm window
