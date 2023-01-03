#include <common.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdexcept>


GLFWGuard::GLFWGuard()
{
	if (!glfwInit())
	{
		throw std::runtime_error{ "Failed to initialize GLFW" };
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}


GLFWGuard::~GLFWGuard()
{
	glfwTerminate();
}


void init_glew()
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error{ "Failed to initialize GLEW" };
	}
}
