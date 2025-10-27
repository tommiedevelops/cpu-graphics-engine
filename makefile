CC = gcc
CFLAGS = -I/opt/homebrew/include -I/opt/homebrew/include/SDL2
LDFLAGS = -L/opt/homebrew/lib -lSDL2 -lm -Iinclude -Icore/math -Icore/renderer -Icore/asset_manager -Icore/scene_manager -Icore/platform -Iapp/ -Icore -Icore/third_party 
DEBUGFLAGS = -fsanitize=address -g
SRC_FILES := core/math/*.c core/renderer/*.c core/platform/*.c core/scene_manager/*.c core/asset_manager/*.c app/*.c 
TEST_SRC_FILES := core/math/*.c core/renderer/*.c core/platform/*.c core/scene_manager/*.c core/asset_manager/*.c app/app.c
TEST_FILES := $(wildcard tests/src/*.c)

.PHONY: run build

build:
	$(CC) $(SRC_FILES) $(DEBUGFLAGS) -o ./build/window $(CFLAGS) $(LDFLAGS)
clean:
	rm ./core/build/window ./tests/build/testing
run:
	ASAN_OPTIONS=detect_leaks=1:leak_check_at_exit=0 ./build/window
tbuild:
		$(CC) $(TEST_FILES) $(TEST_SRC_FILES) $(LDFLAGS) -Itests/include $(DEBUGFLAGS) -o ./build/testing $(CFLAGS) $(LDFLAGS)
trun:
		ASAN_OPTIONS=detect_leaks=1:leak_check_at_exit=0 ./build/testing
docs:
	pdflatex docs/docs-latex/main.tex
	latexmk -pdf -pvc docs/docs-latex/main.tex 

