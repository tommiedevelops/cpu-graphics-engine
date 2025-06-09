CC = gcc
CFLAGS = -I/opt/homebrew/include -I/opt/homebrew/include/SDL2
LDFLAGS = -L/opt/homebrew/lib -lSDL2 -lm
all:
	$(CC) window.c line.c -o window $(CFLAGS) $(LDFLAGS)
clean: 
	rm window
