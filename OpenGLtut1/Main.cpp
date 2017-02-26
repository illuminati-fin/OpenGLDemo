// GLEW (always include GLEW beforw GLFW)
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>

// FUNCTION DELARATIONS
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
std::string readFile(const char *filePath);

int main()
{
	//------------------ SETUP ---------------//
	// Init GLFW
	glfwInit();

	// Use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Avoid legacy, use core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Don't let user resize rendering window
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//------------------ CREATE WINDOW ---------------//
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Test", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Experimental => using modern stuff. Needed for GLFW_OPENGL_CORE_PROFILE
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to intilize GLEW" << std::endl;
		return -1;
	}

	// Create viewport aka rendering window
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	//------------------ CALLBACKS ---------------//
	glfwSetKeyCallback(window, key_callback);


	//------------------ GAME LOOP ---------------//
	while (!glfwWindowShouldClose(window))
	{
		// Event processing like mouse or keyboardinput
		glfwPollEvents();

		// Rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Create triangle vertices
		GLfloat vertices[] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f
		};

		// Init vertex buffer object
		GLuint VBO;
		glGenBuffers(1, &VBO);

		// Select right type buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// Move vertex data to buffer
		// GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
		// GL_DYNAMIC_DRAW : the data is likely to change a lot.
		// GL_STREAM_DRAW : the data will change every time it is drawn.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Read vertex shader source
		std::string vertShaderStr = readFile("VertexShader.vert");
		const char *vertShaderSrc = vertShaderStr.c_str();

		// Create vertex shader
		GLuint vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertShaderSrc, NULL);
		glCompileShader(vertexShader);

		// Check if shader compilation is succesfull
		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER:VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}


		// Swaps color buffer => draws frame. Read about double buffer at: http://www.learnopengl.com/#!Getting-started/Hello-Window
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

std::string readFile(const char *filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}