**A software rasterizer written in C**  

**SETUP**  
Please install: 
- sdl2 version 2.26.5

If using linux: 
- sudo apt install libsdl2-dev

should work. Otherwise just google it.

To launch the program simply clone the repo and run the following commands in the directory:
- make build
- make run-dirty (ignores ASAN warnings)

Use the following command to clean up the directory afterwards.
- make clean

**Note on ASAN**  
- Please note that SDL2 has in-built memory leaks so you may need to run the program multiple times to prevent ASAN from complaining. Pay close attention to memory leak details as it may not pertain to your actual code. 
- It may start spamming ASAN error messages. Just quit and restart.

**For Drawing Lines:**
- Please use the following format in the line.input file:  
Line 1: [int: number of lines]  
Line 2-n: {[int:x0],[int:y0]} {[int:x1],[int:y1]}
- Please ensure the two coordinates are delimited by exactly one space ( ) and within the braces ({}), each integer is delimited by exactly one comma (,). My input parser is not very resilient.

**A note on the line drawing algorithm:**  
- Bresenham's Line Drawing algorithm is definitely more efficient as it does not involve floating point operations and division. However it is more complicated to understand and obscures the theory. 
- I have not implemented it yet as I would like to move on quickly but I may return to it. Please see the relevant code in compute_line.c
