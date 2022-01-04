#include <camera.h>
#include <shader.h>
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

// function headers for window functions.
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// function headers for skybox functions.
unsigned int setup_skybox();
void draw_skybox(unsigned int skyboxTex, unsigned int skyboxVAO);

const bool cube_options = false;

const int window_w = 600; 
const int window_h = 600;

// cam initialisation.
Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = window_w / 2.0f;
float lastY = window_h / 2.0f;
bool firstMouse = true;

// time between frames.
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

const vector<std::string> cubemap_paths
{
	"../res/textures/skybox/right.jpg",
    "../res/textures/skybox/left.jpg",
    "../res/textures/skybox/top.jpg",
    "../res/textures/skybox/bottom.jpg",
    "../res/textures/skybox/front.jpg",
	"../res/textures/skybox/back.jpg"
};


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
	
	//Shader skybox_shader("../shaders/skybox.vert", "../shaders/skybox.frag");
	Shader shader_prog = cube_options ? Shader("../shaders/environment.vert", "../shaders/cubic.frag") : Shader("../shaders/environment.vert", "../shaders/equirectangular.frag");
	
	//unsigned int skyboxVAO = setup_skybox();

	// TRANSFORMATION STUFF BEGINS
	//
	glm::mat4 model_mat = glm::mat4(1.0f);
	
	glm::mat4 proj_mat = glm::perspective(glm::radians(cam.Zoom), (float)window_w / (float)window_h, 0.1f, 100.0f);
	glm::mat4 view_mat = cam.GetViewMatrix();
	

	shader_prog.use();
	shader_prog.setMat4("model", model_mat);
	shader_prog.setMat4("proj", proj_mat);
	shader_prog.setMat4("view", view_mat);

	//skybox_shader.use();
	//skybox_shader.setMat4("proj", proj_mat);
	//skybox_shader.setMat4("view", view_mat);
	// TRANSFORMATION STUFF END
	

	// TEXTURE STUFF BEGINS
	//
	// generating glTexture
	unsigned int textureID;
	glGenTextures(1, &textureID);
	// set texture wrapping/filtering options.
	
	if(cube_options) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	} else {
		glBindTexture(GL_TEXTURE_2D, textureID); 
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
		
	// loading texture
	int width, height, nrChannels;
	unsigned char *data;
	if(cube_options) {
		for(unsigned int i = 0; i < cubemap_paths.size(); i++) {
			data = stbi_load(cubemap_paths[i].c_str(), &width, &height, &nrChannels, 0);
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
		}
	} else {
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("../res/textures/christmas_photo_studio_05_4k.png", &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	stbi_image_free(data);
	
	shader_prog.setInt("tex_env", 0);
	//skybox_shader.setInt("skybox", 0);
	// TEXTURE STUFF END
	
	
	// load a Wavefront Obj file into a Model class.
	Model model = Model("../res/models/cone.obj");

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		float curFrame = glfwGetTime();
		
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;
		
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.4f, 0.4f, 0.75f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		
		//shader_prog.use();
        //shader_prog.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        //shader_prog.setVec3("lightPos", lightPos);
        //shader_prog.setVec3("viewPos", cam.Position);
		
		// pass camera projection matrix to shader every frame.
        glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)window_w / (float)window_h, 0.1f, 100.0f);
        view_mat = cam.GetViewMatrix();
		
		shader_prog.setMat4("proj", projection);
		shader_prog.setMat4("view", view_mat);
		
        //skybox_shader.setMat4("proj", projection);
		//skybox_shader.setMat4("view", view_mat);

		// currently just rotating the cube.
		// model_mat = glm::rotate(model_mat, deltaTime * glm::radians(50.0f), glm::vec3(0.25f, 0.5f, 0.0f));
		// shader_prog.setMat4("model", model_mat);
		
		//glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    	//skybox_shader.use();
    	//draw_skybox(textureID, skyboxVAO);
		
		model.Draw(shader_prog);
		
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


// skybox: sets up the vertex array and buffer objects. 
// Returns: skyboxVAO
// ----------------------------------------------------------------------
unsigned int setup_skybox() {
	float quad[] = {
        // positions          
         1.0f,  1.0f, 1.0f,
         1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
		-1.0f,  1.0f, 1.0f
    };

	unsigned int indices[] = {
    	0, 1, 3,
    	1, 2, 3
	};
   	
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), &quad, GL_STATIC_DRAW);
    
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

	return VAO;
}

void draw_skybox(unsigned int skyboxTex, unsigned int skyboxVAO) {
    unsigned int indices[] = {
    	0, 1, 3,
    	1, 2, 3
	};
   	// skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}
