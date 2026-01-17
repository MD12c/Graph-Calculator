#include "Setup.h"

Setup::Setup(const char* WINDOW_NAME, unsigned int width, unsigned int height, GLfloat RED, GLfloat GREEN, GLfloat BLUE)
	: m_WINDOW_NAME(WINDOW_NAME),
	m_width(width),
	m_height(height),
	m_RED(RED),
	m_GREEN(GREEN),
	m_BLUE(BLUE)
{
}


void Setup::glfwSetup() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_window = glfwCreateWindow(m_width, m_height, m_WINDOW_NAME, NULL, NULL);
	if (m_window == NULL)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}
	glfwMakeContextCurrent(m_window);
	gladLoadGL();
	glViewport(0, 0, m_width, m_height);
	glClearColor(m_RED, m_GREEN, m_BLUE, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(m_window);
};

GLFWwindow* Setup::getWindow() {
	return m_window;
}

Setup::~Setup() {
	if (m_window) {
		glfwDestroyWindow(m_window);
	}
	glfwTerminate();
}

void Setup::glClearCurrentColor() {
	glClearColor(m_RED, m_GREEN, m_BLUE, 1.0f);
}