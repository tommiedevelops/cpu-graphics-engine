CC = gcc
CFLAGS = -I/opt/homebrew/include -I/opt/homebrew/include/SDL2
LDFLAGS = -L/opt/homebrew/lib -lSDL2 -lm
DEBUGFLAGS = -fsanitize=address
all:
	$(CC) window.c line.c inputparser.c render.c $(DEBUGFLAGS) -o window $(CFLAGS) $(LDFLAGS)
clean:
	rm window
