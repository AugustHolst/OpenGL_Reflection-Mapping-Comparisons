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
#include <glm/gtx/string_cast.hpp>

#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

// function headers for window functions.
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void render_controls(GLFWwindow* window, int key, int scancode, int action, int mods);

// function headers for skybox functions.
unsigned int setup_skybox();
void draw_skybox(unsigned int skyboxTex, unsigned int skyboxVAO);

// function header for texture functions.
void load_texture(string tex_path, Shader shader, string uniform_tag);
void load_cube_textures(vector<std::string>, Shader cubic_shader);
void load_paraboloid_textures(string tex_path, Shader shader);

// window resolution.
const int window_w = 600; 
const int window_h = 600;

// cam initialisation.
Camera cam(glm::vec3(0.0f, 1.0f, 7.5f));
float lastX = window_w / 2.0f;
float lastY = window_h / 2.0f;
bool firstMouse = true;

// index for current shader 
// (0:sphere map, 1:equirectangular, 2:cube map, 3:dual paraboloid map)
static const int shader_amount = 4;
static int current_shader = 0;

static const int model_amount = 8;
static int current_model = 0;

// time between frames.
double deltaTime = 0.0f;
double lastFrame = 0.0f;

const vector<std::string> cubemap_paths
{
	"../res/textures/skybox/right.jpg",
    "../res/textures/skybox/left.jpg",
    "../res/textures/skybox/top.jpg",
    "../res/textures/skybox/bottom.jpg",
    "../res/textures/skybox/front.jpg",
	"../res/textures/skybox/back.jpg"
};

/* *
 * In the main function there is outcommented code: 
 * 		1. for rendering a skybox (based on the cubemap textures).
 * 		2. 
 * 		3. FPS_counter function call.
 * 		4. for creating benchmarks.
 * */


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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, render_controls);

	gladLoadGL();
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
	
	
	// Shader initialisation
	// ---------------------
	Shader skybox_shader("../shaders/skybox.vert", "../shaders/skybox.frag");

	Shader spherical_shader("../shaders/environment.vert", "../shaders/spherical.frag");
	Shader equirectangular_shader("../shaders/environment.vert", "../shaders/equirectangular.frag");
	Shader cubic_shader("../shaders/environment.vert", "../shaders/cubic.frag");
	Shader parabolic_shader("../shaders/dual_parabolic.vert", "../shaders/dual_parabolic_rect.frag");
	
	vector<Shader> shaders;
	shaders.push_back(spherical_shader);
	shaders.push_back(equirectangular_shader);
	shaders.push_back(cubic_shader);
	shaders.push_back(parabolic_shader);

	
	// Setting up model, view and projection matrix.
	// ---------------------------------------------
	glm::mat4 model_mat = glm::mat4(0.5f);
	glm::mat4 proj_mat = glm::perspective(glm::radians(cam.Zoom), (float)window_w / (float)window_h, 0.1f, 100.0f);
	glm::mat4 view_mat = cam.GetViewMatrix();
	
	skybox_shader.setMat4("view", view_mat);
	skybox_shader.setMat4("proj", proj_mat);

	for(int i = 0; i < shaders.size(); i++) {
		shaders[i].use();
		shaders[i].setMat4("model", model_mat);
		shaders[i].setMat4("view", view_mat);
		shaders[i].setMat4("proj", proj_mat);
	}

	// assigning textures
	// ------------------
	load_texture("../res/textures/sphere.jpg", spherical_shader, "tex_env");
	load_texture("../res/textures/equirectangular.png", equirectangular_shader, "tex_env");
	load_cube_textures(cubemap_paths, cubic_shader);
	load_paraboloid_textures("../res/textures/paraboloids.jpg", parabolic_shader);
	

	// load a Wavefront Obj file into a Model class.
	vector<Model> models;
	models.push_back(Model("../res/models/cone.obj"));
	models.push_back(Model("../res/models/cube.obj"));
	models.push_back(Model("../res/models/teapot.obj"));
	models.push_back(Model("../res/models/dragon.obj"));
	models.push_back(Model("../res/models/sphere_960.obj"));
	models.push_back(Model("../res/models/sphere_63488.obj"));
	models.push_back(Model("../res/models/sphere_116160.obj"));
	models.push_back(Model("../res/models/sphere_1045440.obj"));
	
	int loop_counter = 0;
	std::chrono::duration<double> testtime(1.0);
	std::chrono::duration<double>::rep nulo = std::chrono::duration<double> (0.0).count();
	//std::ofstream benchmark_file;
	//benchmark_file.open("benchmark.txt", std::ios_base::app);
	
	// should turn off v-sync, allowing the fps to go uncapped. (Not sure if hardware specific)
	//glfwSwapInterval(0);
	
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		
		float current_time = glfwGetTime();
		deltaTime = current_time - lastFrame;
		lastFrame = current_time;
		
		// FPS_counter(loop_counter, deltaTime);
		
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.4f, 0.4f, 0.75f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		// pass camera projection matrix to shader every frame.
        proj_mat = glm::perspective(glm::radians(cam.Zoom), (float)window_w / (float)window_h, 0.1f, 100.0f);
        view_mat = cam.GetViewMatrix();

		//skybox_shader.use();
		//skybox_shader.setMat4("proj", proj_mat);
		//skybox_shader.setMat4("view", view_mat);
    	//draw_skybox(cubeMapTex, skyboxVAO);

		shaders[current_shader].use();
		shaders[current_shader].setMat4("proj", proj_mat);
		shaders[current_shader].setMat4("view", view_mat);
		if(current_shader == 4) 
			shaders[current_shader].setVec3("cam_pos", cam.Position);

		//std::chrono::time_point<std::chrono::system_clock> start, end;
		//start = std::chrono::system_clock::now();

		models[current_model].Draw(shaders[current_shader]);
		
		//end = std::chrono::system_clock::now();
		//std::chrono::duration<double> render_duration = end - start;
		//benchmark_file << render_duration.count() << std::endl;
		//if(++loop_counter == 1000) break;

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

void render_controls(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_J && action == GLFW_PRESS) {
		(current_model + 1 >= model_amount) ? : current_model++;
    }
	if (key == GLFW_KEY_K && action == GLFW_PRESS) {
		(current_model - 1 < 0) ? : current_model--;
    }
    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		(current_shader + 1 >= shader_amount) ? : current_shader++;
    }
	if (key == GLFW_KEY_H && action == GLFW_PRESS) {
		(current_shader - 1 < 0) ? : current_shader--;
	}
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
    glDepthMask(GL_FALSE);
    
	glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0 + skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
    glBindVertexArray(0);
    
	glDepthMask(GL_TRUE); 
}

void load_texture(string tex_path, Shader shader, string uniform_tag) {
	unsigned int texID;
	glGenTextures(1, &texID);
	glActiveTexture(GL_TEXTURE0 + texID);
	glBindTexture(GL_TEXTURE_2D, texID); 
		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	
	int width, height, nrChannels;
	unsigned char *data;
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(tex_path.c_str(), &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	stbi_set_flip_vertically_on_load(false);
	
	shader.use();
	shader.setInt(uniform_tag.c_str(), texID);
}

void load_cube_textures(vector<std::string> cubemap_paths, Shader shader) {
	unsigned int texID;
	glGenTextures(1, &texID);
	glActiveTexture(GL_TEXTURE0 + texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	int width, height, nrChannels;
	unsigned char *data;
	for(unsigned int i = 0; i < cubemap_paths.size(); i++) {
		data = stbi_load(cubemap_paths[i].c_str(), &width, &height, &nrChannels, 0);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
		);
		stbi_image_free(data);
	}
	
	shader.use();
	shader.setInt("cubemap", texID);
}

void load_paraboloid_textures(string tex_path, Shader shader) {
	unsigned int texID;
	glGenTextures(1, &texID);
	glActiveTexture(GL_TEXTURE0 + texID);
	glBindTexture(GL_TEXTURE_RECTANGLE, texID);
	
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	int width, height, nrChannels;
	unsigned char *data;
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(tex_path.c_str(), &width, &height, &nrChannels, 0);
	glTexImage2D(
		GL_TEXTURE_RECTANGLE, 
		0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
	);
	stbi_image_free(data);
	stbi_set_flip_vertically_on_load(false);

	shader.use();
	shader.setInt("paraboloids", texID);
}

void FPS_counter(int loop_counter, double deltaTime) {
	loop_counter++;
	if (deltaTime >= 1.0 / 30.0) {
		std::string FPS = std::to_string((1.0 / deltaTime) * loop_counter);
		std::cout << FPS << std::endl;
	}
}
