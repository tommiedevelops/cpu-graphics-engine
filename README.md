A software rasterizer written in C

Please install: 
- sdl2 version 2.26.5

If using linux: 
- sudo apt install libsdl2-dev

should work. Otherwise just google it.

To launch the program simply clone the repo and run the following commands in the directory:
- make build
- make run

Use the following command to clean up the directory afterwards.
- make clean

Please note that SDL2 has in-built memory leaks so pay close attention to the leak information to actual debug the program.

For Drawing Lines:
- Please use the following format in the line.input file:
	line 1: [integer value for the number of lines]  
	line 1-n: {[int], [int]} {[int], [int]}    
 where the first set of integers are the start coords and the second set of ints are the end coords for the line.
