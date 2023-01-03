#ifndef _CG_WINDOW_H
#define _CG_WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>

struct GLFWwindow;

class Window
{
public:
	using KeyCallback = void(GLFWwindow *, int, int, int, int);

	Window(int width, int height, const char *title);
	~Window();

	bool is_open() const;
	GLFWwindow* get_handle() const;

	void clear();
	void draw(GLenum mode);
	void display();
	void register_key_callback(KeyCallback callback);
	void handle_events();

private:
	void compile_shaders();
	void prepare_vao();

private:
	GLFWwindow* window_;
	GLuint shader_program_;
	GLuint vao_id_;
};

#endif // #ifndef _CG_WINDOW_H
