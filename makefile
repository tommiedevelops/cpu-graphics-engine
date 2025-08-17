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
	ASAN_OPTIONS=detect_leaks=1:leak_check_at_exit=0 ./build/window $(ARGS)
run-normal:
	./build/window $(ARGS)
tbuild:
		$(CC) $(TEST_FILES) $(SRC_FILES) -Iinclude -Itests/include $(DEBUGFLAGS) -o ./tests/build/testing $(CFLAGS) $(LDFLAGS)
trun:
		ASAN_OPTIONS=detect_leaks=1:leak_check_at_exit=0 ./tests/build/testing
trun-normal:
	./tests/build/testing
