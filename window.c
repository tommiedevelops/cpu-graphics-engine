#include <stdio.h>
#include <GLFW/glfw3.h>

#define WIDTH (800) // pixels
#define HEIGHT (600)
#define WINDOW_NAME ("Renderer")

int main(void) {
	GLFWwindow* window;
	uint32_t framebuffer[WIDTH*HEIGHT] = {0}; //buffer for what will be drawn to screen

	printf("width={%d}, height={%d}\n", WIDTH, HEIGHT);

	// Initialising GLFW window
	if(!glfwInit()) {return -1;}
	window = glfwCreateWindow(WIDTH,HEIGHT, WINDOW_NAME, NULL, NULL);
	if(!window) { glfwTerminate(); return -1;}
	glfwMakeContextCurrent(window);

	//Drawing a single red pixel to the middle of the screen
	int x = 200;
	int y = 200;
	framebuffer[y * WIDTH + x] = 0xFF0000FF;

	// upload framebuffer to a texture
	GLuint tex; // to store the texture handle
	glGenTextures(1, &tex); // openGL generates the handle and stores it here
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, framebuffer);

	// Select color of background
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Rendering loop
	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, framebuffer);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();	
	return 0;
}
