#include <headers/shader_s.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <cstdio>
#include <cstdlib>

int main(int argc, const char** argv) {
	GLFWwindow* window;

	if (!glfwInit()) { 
		printf("Could not init glfw"); 
		return 1;
	}

	window = glfwCreateWindow(640, 480, "Bsc", NULL, NULL);
	if (!window) {
		printf("Could not create glfw window");
		return 1;
	}

	// build and compile our shader program
	// ------------------------------------
	//Shader ourShader("shaders\vert.vs", "shaders\frag.fs");

	unsigned char* data = new unsigned char[100 * 100 * 3];
	for (int y = 0; y < 100; y++) {
		for (int x = 0; x < 100; x++) {
			data[y * 100 * 3 + x * 3		] = 0xff;
			data[y * 100 * 3 + x * 3 + 1	] = 0x00;
			data[y * 100 * 3 + x * 3 + 2	] = 0x00;
		}
	}

	glfwMakeContextCurrent(window);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	gladLoadGL();
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	Shader shader_prog ("shaders\vert.vs", "shaders\frag.fs");
	float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	while (!glfwWindowShouldClose(window)) {
		glClearColor(79.0f, 29.0f, 130.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		shader_prog.use();
		color[2] = sin(glfwGetTime()) / 2.0f + 0.5f;
		shader_prog.setVec4("ourColor", color);
		
		glfwSwapBuffers(window);
		glfwWaitEvents();
	} glfwTerminate();
	return 0;
}