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

	glfwMakeContextCurrent(window);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	gladLoadGL();
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	Shader shader_prog ("../shaders/vert.vs", "../shaders/frag.fs");
	float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	while (!glfwWindowShouldClose(window)) {
		glClearColor(79.0f, 29.0f, 130.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		glBindVertexArray(VAO);
		// 2. copy our vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// 3. then set our vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		float now = sin(glfwGetTime());
		color[1] = now / 2.0f + 0.5f;
		shader_prog.setVec4("ourColor", color);

		shader_prog.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glfwSwapBuffers(window);
		glfwWaitEvents();
	} glfwTerminate();
	return 0;
}