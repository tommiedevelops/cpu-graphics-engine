CC = gcc
CFLAGS = -I/opt/homebrew/include -I/opt/homebrew/include/SDL2
LDFLAGS = -L/opt/homebrew/lib -lSDL2 -lm -Iinclude -I_core/math -I_core/renderer -I_core/scene -I_core/window -I_app/ -I_core -I_core/third_party
DEBUGFLAGS = -fsanitize=address
SRC_FILES := _core/math/*.c _core/renderer/*.c _core/window/*.c _core/*.c _core/scene/*.c _app/*.c
TEST_FILES := $(wildcard tests/_core/*.c)

.PHONY: run

build:
	$(CC) $(SRC_FILES) $(DEBUGFLAGS) -o ./_core/build/window $(CFLAGS) $(LDFLAGS)
clean:
	rm ./_core/build/window ./tests/build/testing
run:
	ASAN_OPTIONS=detect_leaks=1:leak_check_at_exit=0 ./_core/build/window
tbuild:
		$(CC) $(TEST_FILES) $(SRC_FILES) $(LDFLAGS) -I_tests/include $(DEBUGFLAGS) -o ./_tests/build/testing $(CFLAGS) $(LDFLAGS)
trun:
		ASAN_OPTIONS=detect_leaks=1:leak_check_at_exit=0 ./_tests/build/testing
docs:
	pdflatex docs/docs-latex/main.tex
	latexmk -pdf -pvc docs/docs-latex/main.tex 

