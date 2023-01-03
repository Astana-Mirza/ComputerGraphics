#include <window.h>

#include <stdexcept>

Window::Window(int width, int height, const char* title) : window_{nullptr}
{
	window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window_)
	{
		throw std::runtime_error{"Failed to open GLFW window."};
	}
	glfwMakeContextCurrent(window_);
	glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE);

	glViewport(0, 0, width, height);
}


Window::~Window()
{
	glfwDestroyWindow(window_);
}


GLFWwindow* Window::get_handle() const
{
	return window_;
}


bool Window::is_open() const
{
	return (glfwWindowShouldClose(window_) == 0);
}


void Window::clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}


void Window::display()
{
	glfwSwapBuffers(window_);
}


void Window::register_key_callback(KeyCallback callback)
{
	glfwSetKeyCallback(window_, callback);
}


void Window::handle_events()
{
	glfwPollEvents();
}
