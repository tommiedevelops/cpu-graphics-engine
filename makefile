CC = gcc
CFLAGS = -I/opt/homebrew/include -I/opt/homebrew/include/SDL2
LDFLAGS = -L/opt/homebrew/lib -lSDL2 -lm -Iinclude
DEBUGFLAGS = -fsanitize=address
SRC_FILES := $(filter-out src/main.c, $(wildcard src/*.c))
TEST_FILES := $(wildcard tests/src/*.c)

build: ./build/window
./build/window:
	$(CC) src/*.c $(DEBUGFLAGS) -o ./build/window $(CFLAGS) $(LDFLAGS)
clean:
	rm ./build/window ./tests/build/testing
run:
	./build/window

