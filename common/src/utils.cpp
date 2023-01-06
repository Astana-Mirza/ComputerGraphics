#include <common/utils.h>

#include <stdexcept>

#if defined(USE_GLFW)

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

#endif // USE_GLFW


#if defined(USE_GLEW)

void init_glew()
{
	// glewExperimental = GL_TRUE;    // not needed since GLEW 2.0.0
	if (glewInit() != GLEW_OK || !GLEW_VERSION_3_3)
	{
		throw std::runtime_error{ "Failed to initialize GLEW" };
	}
}

#endif // USE_GLEW
