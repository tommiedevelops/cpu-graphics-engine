**A software rasterizer written in C**  

![Demo of Software Rasterizer](thumbnail.gif)

**SETUP**  
Please install: 
- sdl2 version 2.26.5

If using linux: 
- sudo apt install libsdl2-dev

should work. Otherwise just google it.

To launch the program simply clone the repo and run the following commands in the directory:
- make build
- make run ARGS=bunny

Use the following command to clean up the directory afterwards.
- make clean

**Note on ASAN**  
- Please note that SDL2 has in-built memory leaks so you may need to run the program multiple times to prevent ASAN from complaining. Pay close attention to memory leak details as it may not pertain to your actual code. 
- It may start spamming ASAN error messages. Just quit and restart.

