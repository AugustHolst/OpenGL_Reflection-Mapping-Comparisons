#include <shader_s.h>
#include <hello_s.h>

#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

int main(int argc, const char** argv) {
	if (!glfwInit()) { 
		printf("Could not init glfw"); 
		return 1;
	}
	
	const int window_w = 640; const int window_h = 480;
	
	GLFWwindow* window = glfwCreateWindow(window_w, window_h, "Bsc", NULL, NULL);
	if (!window) {
		printf("Could not create glfw window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	gladLoadGL();
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
	
	Shader shader_prog("../shaders/vert.vs", "../shaders/frag.fs");
	
/* 	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.2f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.2f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	}; */
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	// For element buffer object
	unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
	
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. then set our element buffer object. Meaning specify the face indices.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 4. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);	
	
	// TRANSFORMATION STUFF BEGINS
	//
	glm::mat4 model = glm::mat4(1.0f);
	
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)window_w / (float)window_h, 0.1f, 100.0f);
	
	glm::mat4 projview = projection * view;
	//glm::mat4 trans = glm::mat4(1.0f);
	
	//trans *= glm::scale(trans, glm::vec3( 2.0f, 2.0f, 2.0f));
	
	shader_prog.use();	
	shader_prog.setMat4("projview", projview);
	//shader_prog.setMat4("view", view);
	shader_prog.setMat4("model", model);

	// TEXTURE STUFF BEGINS
	//
	Hello hello("Mark");
	
	// generating glTexture
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); 
	// set texture wrapping/filtering options.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// loading texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("../res/cowbitch.jpg", &width, &height, &nrChannels, 0);
	//	note: glTexImage2D()
	//(a:3) format to store. (a:6) always 0, legacy stuff. (a:7) source format (a:8) data type of source.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	stbi_image_free(data);
	
	shader_prog.setInt("texture1", 0);
	// TEXTURE STUFF END
	
	glfwFocusWindow(window);
	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
		
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.4f, 0.4f, 0.75f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glBindVertexArray(VAO);
		
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.25f, 0.5f, 0.0f));
		shader_prog.setMat4("model", model);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	} glfwTerminate();
	return 0;
}