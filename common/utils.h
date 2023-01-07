#ifndef _CG_UTILS_H
#define _CG_UTILS_H

#include <cg_config.h>

#if defined(USE_GLAD)
#   include <glad/glad.h>
#elif defined(USE_GLEW)
#   include <GL/glew.h>
#endif

#if defined(USE_GLFW)
#   include <GLFW/glfw3.h>
#endif

#if defined(USE_GLFW)

struct GLFWGuard
{
	GLFWGuard();
	~GLFWGuard();
};

#endif // USE_GLFW


#if defined(USE_GLEW)
// must be called after OpenGL context creation
void init_glew();
#endif // USE_GLEW

#endif // #ifndef _CG_UTILS_H