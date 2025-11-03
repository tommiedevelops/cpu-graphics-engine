CC = gcc
CFLAGS = -I/opt/homebrew/include -I/opt/homebrew/include/SDL2
LDFLAGS = -L/opt/homebrew/lib -lSDL2 -lm -Iinclude -Icore/include -Icore/src -Iapp

DEBUGFLAGS = -fsanitize=address -g
SRC_FILES := core/src/game_math/*.c core/src/renderer/*.c core/src/platform/*.c app/*.c core/src/scene_manager/*.c core/src/asset_manager/*.c
TEST_SRC_FILES := core/src/game_math/*.c core/src/renderer/*.c core/src/platform/*.c app/app.c core/src/scene_manager.c core/src/asset_manager.c
TEST_FILES := $(wildcard tests/src/*.c)

.PHONY: run build

build:
	$(CC) $(SRC_FILES) $(DEBUGFLAGS) -o ./build/window $(CFLAGS) $(LDFLAGS)
clean:
	rm ./build/window ./build/testing
run:
	ASAN_OPTIONS=detect_leaks=1:leak_check_at_exit=0 ./build/window
tbuild:
		$(CC) $(TEST_FILES) $(TEST_SRC_FILES) $(LDFLAGS) -Itests/include $(DEBUGFLAGS) -o ./build/testing $(CFLAGS) $(LDFLAGS)
trun:
		ASAN_OPTIONS=detect_leaks=1:leak_check_at_exit=0 ./build/testing
docs:
	pdflatex docs/docs-latex/main.tex
	latexmk -pdf -pvc docs/docs-latex/main.tex 

