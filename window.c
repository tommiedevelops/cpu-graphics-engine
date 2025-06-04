#include <stdio.h>
#include <GLFW/glfw3.h>

#define SCREEN_WIDTH (800) // pixels
#define SCREEN_HEIGHT (600)

int main(void) {
	GLFWwindow* window;	
	
	printf("width={%d}, height={%d}\n", SCREEN_WIDTH, SCREEN_HEIGHT);
	
	
	if(!glfwInit()) {return -1;}
	window = glfwCreateWindow(640,480, "Hello World", NULL, NULL);
	
	if(!window) { glfwTerminate(); return -1;}

	glfwMakeContextCurrent(window);

	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();	
	return 0;
}
