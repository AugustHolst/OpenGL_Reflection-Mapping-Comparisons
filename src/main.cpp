#include <camera.h>
#include <shader_s.h>
#include <mesh.h>
#include <model.h>

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const int window_w = 600; 
const int window_h = 600;
//cam initialisation
Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = window_w / 2.0f;
float lastY = window_h / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f;

const glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main(int argc, const char** argv) {
	if (!glfwInit()) { 
		printf("Could not init glfw"); 
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(window_w, window_h, "Bsc", NULL, NULL);
	if (!window) {
		printf("Could not create glfw window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
	// glfw: capture mouse.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	gladLoadGL();
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
	
	//Shader initialisation
	Shader shader_prog("../shaders/phong.vs", "../shaders/phong.fs");
	
	
	// TRANSFORMATION STUFF BEGINS
	//
	glm::mat4 model_mat = glm::mat4(1.0f);
	
	glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)window_w / (float)window_h, 0.1f, 100.0f);
	glm::mat4 view = cam.GetViewMatrix();
	
	shader_prog.use();	
	shader_prog.setMat4("proj", projection);
	shader_prog.setMat4("view", view);
	// TRANSFORMATION STUFF END
	

	// TEXTURE STUFF BEGINS
	//
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
	unsigned char *data = stbi_load("../res/textures/planet.png", &width, &height, &nrChannels, 0);
	//	note: glTexImage2D()
	//(a:3) format to store. (a:6) always 0, legacy stuff. (a:7) source format (a:8) data type of source.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	stbi_image_free(data);
	
	shader_prog.setInt("texture1", 0);
	// TEXTURE STUFF END
	
	
	// load a Wavefront Obj file into a Model class.
	Model sphere = Model("../res/models/sphere_63488.obj");

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		float curFrame = glfwGetTime();
		
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;
		
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.4f, 0.4f, 0.75f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		
		//shader_prog.use();
        shader_prog.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shader_prog.setVec3("lightPos", lightPos);
        shader_prog.setVec3("viewPos", cam.Position);
		
		// pass camera projection matrix to shader every frame.
        glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)window_w / (float)window_h, 0.1f, 100.0f);
        shader_prog.setMat4("proj", projection);
        // camera/view transformation.
        glm::mat4 view = cam.GetViewMatrix();
        shader_prog.setMat4("view", view);
		// currently just rotating the cube.
		model_mat = glm::rotate(model_mat, deltaTime * glm::radians(50.0f), glm::vec3(0.25f, 0.5f, 0.0f));
		shader_prog.setMat4("model", model_mat);
		
		sphere.Draw(shader_prog);

		glfwSwapBuffers(window);
		glfwPollEvents();
	} glfwTerminate();
	return 0;
}

// glfw: whenever a button is pressed. I.E. Keyboard or mouse.
// ---------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.ProcessKeyboard(FORWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.ProcessKeyboard(BACKWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.ProcessKeyboard(LEFT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    cam.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cam.ProcessMouseScroll(yoffset);	
}
