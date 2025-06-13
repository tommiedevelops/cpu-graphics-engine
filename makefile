CC = gcc
CFLAGS = -I/opt/homebrew/include -I/opt/homebrew/include/SDL2
LDFLAGS = -L/opt/homebrew/lib -lSDL2 -lm
DEBUGFLAGS = -fsanitize=address
build:
	$(CC) src/window.c src/line.c src/inputparser.c src/render.c $(DEBUGFLAGS) -o window $(CFLAGS) $(LDFLAGS)
clean:
	rm window testing
run-dirty:
	ASAN_OPTIONS=detect_leaks=1:leak_check_at_exit=0 ./window
run:
	./window
build-tests:
		$(CC) tests/line_tests.c src/line.c src/inputparser.c src/render.c $(DEBUGFLAGS) -o testing $(CFLAGS) $(LDFLAGS)
run-tests:
		ASAN_OPTIONS=detect_leaks=1:leak_check_at_exit=0 ./testing

