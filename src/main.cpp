#include <shader_s.h>
#include <hello_s.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <cstdio>
#include <cstdlib>

int main(int argc, const char** argv) {
	if (!glfwInit()) { 
		printf("Could not init glfw"); 
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(640, 480, "Bsc", NULL, NULL);
	if (!window) {
		printf("Could not create glfw window");
		return 1;
	}

	// build and compile our shader program
	// ------------------------------------
	//Shader ourShader("shaders\vert.vs", "shaders\frag.fs");

	glfwMakeContextCurrent(window);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	gladLoadGL();
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
	
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.4, 0.0, 0.6,
		 0.5f, -0.5f, 0.0f, 0.4, 0.0, 0.6,
		 0.0f,  0.5f, 0.0f, 0.4, 0.0, 0.6
	};
	
	Hello hello("Mark");
	
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);
	
	Shader shader_prog("../shaders/vert.vs", "../shaders/frag.fs");
	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glfwFocusWindow(window);
	while (!glfwWindowShouldClose(window)) {
		
		glClearColor(0.9f, 0.9f, 0.85f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader_prog.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glfwSwapBuffers(window);
		glfwWaitEvents();
	} glfwTerminate();
	return 0;
}