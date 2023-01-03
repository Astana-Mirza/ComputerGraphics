#ifndef _CG_COMMON_H
#define _CG_COMMON_H

struct GLFWGuard
{
	GLFWGuard();
	~GLFWGuard();
};

void init_glew();

#endif // #ifndef _CG_COMMON_H