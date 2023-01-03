#include <iostream>

#include <common.h>
#include <window.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, 1);
		break;
	default:
		break;
	}
}


int main()
{
	try
	{
		GLFWGuard g;
		Window window(800, 600, "Lab1");
		init_glew();


		window.register_key_callback(key_callback);

		while (window.is_open())
		{
			window.clear();

			// render...

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

