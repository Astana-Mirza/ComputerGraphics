#include <window.h>

#include <common.h>
#include <stdexcept>
#include <cstdlib>

namespace
{
	constexpr static char* basic_vertex_shader =
		"#version 330 core\n"
		"layout (location = 0) in vec3 pos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);\n"
		"}\0";

	constexpr static char* basic_fragment_shader =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(0.1f, 0.4f, 1.0f, 1.0f);\n"
		"}\0";

	static GLfloat vertices[30];
}

Window::Window(int width, int height, const char* title):
	window_{ nullptr }, shader_program_{ 0 }, vao_id_{ 0 }
{
	window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window_)
	{
		throw std::runtime_error{"Failed to open GLFW window."};
	}
	glfwMakeContextCurrent(window_);
	glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE);
	init_glew();

	for (int i = 0; i < sizeof(vertices) / sizeof(GLfloat); i++)
	{
		vertices[i] = std::rand() / (RAND_MAX / 2.0f) - 1.0f;
	}

	glViewport(0, 0, width, height);

	compile_shaders();
	prepare_vao();
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


void Window::draw(GLenum mode)
{
	glUseProgram(shader_program_);
	glBindVertexArray(vao_id_);
	glDrawArrays(mode, 0, sizeof(vertices) / sizeof(GLfloat) / 3);
	glBindVertexArray(0);
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


void Window::compile_shaders()
{
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader =  glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragment_shader, 1, &basic_fragment_shader, nullptr);
	glShaderSource(vertex_shader, 1, &basic_vertex_shader, nullptr);

	glCompileShader(vertex_shader);
	glCompileShader(fragment_shader);

	shader_program_ = glCreateProgram();
	glAttachShader(shader_program_, vertex_shader);
	glAttachShader(shader_program_, fragment_shader);
	glLinkProgram(shader_program_);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}


void Window::prepare_vao()
{
	GLuint vbo_id = 0;
	glGenBuffers(1, &vbo_id);

	glGenVertexArrays(1, &vao_id_);
	glBindVertexArray(vao_id_);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
