#include <common.h>
#include <window.h>

#include <iostream>
#include <cstdlib>

namespace
{

static GLenum current_mode = GL_POINTS;

static constexpr GLenum modes[] = {
	  GL_POINTS
	, GL_LINES
	, GL_LINE_STRIP
	, GL_LINE_LOOP
	, GL_TRIANGLES
	, GL_TRIANGLE_STRIP
	, GL_TRIANGLE_FAN
/*	, GL_QUADS			// removed in OpenGL >=3.1
	, GL_QUAD_STRIP
	, GL_POLYGON*/
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	static int i = 0;
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, 1);
		break;
	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS)
		{
			i = (i + 1) % (sizeof(modes) / sizeof(GLenum));
			current_mode = modes[i];
		}
		break;
	default:
		break;
	}
}

}

int main()
{
	std::srand(std::time(nullptr));
	try
	{
		GLFWGuard g;
		Window window(800, 600, "Lab1");

		window.register_key_callback(key_callback);


		while (window.is_open())
		{
			window.clear();

			window.draw(current_mode);

			window.display();
			window.handle_events();
		}
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << "Error: " << err.what() << "\n";
	}

	return 0;
}

