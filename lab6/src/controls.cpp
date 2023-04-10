#include <controls.h>

#include <common/utils.h>
#include <common/shader_program.h>

#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <FL/Fl_Hor_Slider.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Spinner.H>

#include <vector>
#include <cmath>

namespace // data
{

GLuint vao[2];
GLuint vbo[2];
const std::string vertex_shader = R"(
	#version 330 core
	layout (location = 0) in vec3 in_pos;
	out vec4 vertex_color;

	uniform mat4 translate_matr;
	uniform mat4 scale_matr;
	uniform mat4 rotate_matr;

	void main()
	{
		gl_Position = translate_matr * rotate_matr * scale_matr * vec4(in_pos, 1.0);
		vertex_color = vec4(0.8, 0.8, 1.0, 1.0);
	}
)";

const std::string fragment_shader = R"(
	#version 330 core
	out vec4 out_color;

	in vec4 vertex_color;

	void main()
	{
	    out_color = vertex_color;
	} 
)";

const std::string axis_vertex_shader = R"(
	#version 330 core
	layout (location = 0) in vec3 in_pos;
	layout (location = 1) in vec3 in_color;
	out vec4 vertex_color;

	uniform mat4 scale_matr;
	uniform mat4 rotate_matr;

	void main()
	{
		mat4 translate_matr = mat4( 1.0,  0.0,  0.0,  0.0,
								    0.0,  1.0,  0.0,  0.0,
								    0.0,  0.0,  1.0,  0.0,
								   -0.8, -0.8,  0.0,  1.0);
		gl_Position = translate_matr * rotate_matr * scale_matr * vec4(in_pos, 1.0);
		vertex_color = vec4(in_color, 1.0);
	}
)";

ShaderProgram shader;
ShaderProgram axis_shader;
std::vector<glm::vec3> vertices;
GLuint vertices_per_circle = 10;
glm::mat4 translate_matr(1.0f);
glm::mat4 scale_matr(1.0f);
glm::vec3 rotate_angles(0.0f);

constexpr GLuint vertical_count = 6;
constexpr GLfloat circle_offset = 0.04f;							// расстояние между соседними окружностями
const std::vector<float> circle_params = {							// радиусы окружностей
	0.06f, 0.05f, 0.04f, 0.04f, 0.05f, 0.06f, 0.07f, 0.08f, 0.09f, 0.10f,
	0.10f, 0.10f, 0.11f, 0.12f, 0.14f, 0.16f, 0.16f, 0.17f, 0.17f, 0.18f,
	0.18f, 0.18f, 0.18f, 0.17f, 0.16f, 0.16f, 0.14f, 0.12f, 0.12f, 0.12f,
	0.14f, 0.16f, 0.16f, 0.10f, 0.06f, 0.05f, 0.04f, 0.03f, 0.02f, 0.01f
};
const std::vector<float> axis_data = {
	0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.1f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.1f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.1f, 0.0f, 0.0f, 1.0f
};

} // data


namespace // functions
{

void generate_vertices(std::vector<glm::vec3>& vertices)
{
	vertices.clear();
	float y = -circle_offset * circle_params.size() / 2;				// создание окружностей
	for (GLuint i = 0; i < circle_params.size(); i++)
	{
		float x = -circle_params[i];
		for (GLuint j = 0; j < vertices_per_circle / 2; j++)
		{
			vertices.push_back(glm::vec3{ x, y, std::sqrt(circle_params[i] * circle_params[i] - x * x)});
			x += 4 * circle_params[i] / vertices_per_circle;
		}
		x = circle_params[i];
		for (GLuint j = 0; j < vertices_per_circle / 2; j++)
		{
			vertices.push_back(glm::vec3{ x, y, -std::sqrt(circle_params[i] * circle_params[i] - x * x)});
			x -= 4 * circle_params[i] / vertices_per_circle;
		}
		if (vertices_per_circle % 2)
		{
			vertices.push_back(glm::vec3{ -circle_params[i], y, 0 });
		}
		y += circle_offset;
	}

	GLuint vetical_strip = vertices_per_circle / vertical_count;		// создание вертикальных линий
	for (GLuint i = 0; i < vertical_count; i++)
	{
		for (GLuint j = 0; j < circle_params.size(); j++)
		{
			vertices.push_back(vertices[j * vertices_per_circle + i * vetical_strip]);
		}
	}
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
}


template <int N>
void callback_select_translation(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Hor_Slider* slider = static_cast<Fl_Hor_Slider*>(caller);
	translate_matr[3][N] = static_cast<float>(slider->value());
	shader.set_uniform("translate_matr", translate_matr);
	window->get_gl_window()->redraw();
}


template <int N>
void callback_select_scale(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Hor_Slider* slider = static_cast<Fl_Hor_Slider*>(caller);
	scale_matr[N][N] = static_cast<float>(slider->value());
	shader.set_uniform("scale_matr", scale_matr);
	axis_shader.set_uniform("scale_matr", scale_matr);
	window->get_gl_window()->redraw();
}


template <int N>
void callback_select_rotation(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Hor_Slider* slider = static_cast<Fl_Hor_Slider*>(caller);
	rotate_angles[N] = static_cast<float>(slider->value());
	glm::mat4 rotate_matr(1.0f);
	rotate_matr = glm::rotate(rotate_matr, glm::radians(rotate_angles.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotate_matr = glm::rotate(rotate_matr, glm::radians(rotate_angles.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotate_matr = glm::rotate(rotate_matr, glm::radians(rotate_angles.z), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.set_uniform("rotate_matr", rotate_matr);
	axis_shader.set_uniform("rotate_matr", rotate_matr);
	window->get_gl_window()->redraw();
}


void callback_select_detalization(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Spinner* spinner = static_cast<Fl_Spinner*>(caller);
	vertices_per_circle = static_cast<GLuint>(spinner->value()) * 10;
	generate_vertices(vertices);
	window->get_gl_window()->redraw();
}

} // functions


void spawn_translation_control(MainWindow<GLWindow>& window, int x, int y)
{
	Fl_Box* translate_label = new Fl_Box(x, y, 220, 20, "Translation");
	Fl_Hor_Slider* slider_tr_x = new Fl_Hor_Slider(x, y + 30, 220, 20, "X");
	Fl_Hor_Slider* slider_tr_y = new Fl_Hor_Slider(x, y + 80, 220, 20, "Y");
	Fl_Hor_Slider* slider_tr_z = new Fl_Hor_Slider(x, y + 130, 220, 20, "Z");

	slider_tr_x->bounds(-1, 1);
	slider_tr_y->bounds(-1, 1);
	slider_tr_z->bounds(-1, 1);

	slider_tr_x->callback(callback_select_translation<0>, &window);
	slider_tr_y->callback(callback_select_translation<1>, &window);
	slider_tr_z->callback(callback_select_translation<2>, &window);

	window.get_scroll()->add(translate_label);
	window.get_scroll()->add(slider_tr_x);
	window.get_scroll()->add(slider_tr_y);
	window.get_scroll()->add(slider_tr_z);
}


void spawn_scale_control(MainWindow<GLWindow>& window, int x, int y)
{
	Fl_Box* translate_label = new Fl_Box(x, y, 220, 20, "Scale");
	Fl_Hor_Slider* slider_sc_x = new Fl_Hor_Slider(x, y + 30, 220, 20, "X");
	Fl_Hor_Slider* slider_sc_y = new Fl_Hor_Slider(x, y + 80, 220, 20, "Y");
	Fl_Hor_Slider* slider_sc_z = new Fl_Hor_Slider(x, y + 130, 220, 20, "Z");

	slider_sc_x->bounds(-3, 3);
	slider_sc_y->bounds(-3, 3);
	slider_sc_z->bounds(-3, 3);

	slider_sc_x->value(1);
	slider_sc_y->value(1);
	slider_sc_z->value(1);

	slider_sc_x->callback(callback_select_scale<0>, &window);
	slider_sc_y->callback(callback_select_scale<1>, &window);
	slider_sc_z->callback(callback_select_scale<2>, &window);

	window.get_scroll()->add(translate_label);
	window.get_scroll()->add(slider_sc_x);
	window.get_scroll()->add(slider_sc_y);
	window.get_scroll()->add(slider_sc_z);
}


void spawn_rotation_control(MainWindow<GLWindow>& window, int x, int y)
{
	Fl_Box* translate_label = new Fl_Box(x, y, 220, 20, "Rotation");
	Fl_Hor_Slider* slider_rot_x = new Fl_Hor_Slider(x, y + 30, 220, 20, "X");
	Fl_Hor_Slider* slider_rot_y = new Fl_Hor_Slider(x, y + 80, 220, 20, "Y");
	Fl_Hor_Slider* slider_rot_z = new Fl_Hor_Slider(x, y + 130, 220, 20, "Z");

	slider_rot_x->bounds(-180, 180);
	slider_rot_y->bounds(-180, 180);
	slider_rot_z->bounds(-180, 180);

	slider_rot_x->value(30);
	slider_rot_y->value(-15);

	slider_rot_x->callback(callback_select_rotation<0>, &window);
	slider_rot_y->callback(callback_select_rotation<1>, &window);
	slider_rot_z->callback(callback_select_rotation<2>, &window);

	window.get_scroll()->add(translate_label);
	window.get_scroll()->add(slider_rot_x);
	window.get_scroll()->add(slider_rot_y);
	window.get_scroll()->add(slider_rot_z);
}


void spawn_detalization_control(MainWindow<GLWindow>& window, int x, int y)
{
	Fl_Spinner* spinner = new Fl_Spinner(x + 150, y, 70, 30, "Detalization level:  ");
	spinner->type(FL_INT_INPUT);
	spinner->minimum(1);
	spinner->maximum(10);
	spinner->value(2);
	spinner->callback(callback_select_detalization, &window);
	window.get_scroll()->add(spinner);
}


void draw_init()
{
	glGenVertexArrays(2, vao);
	glGenBuffers(2, vbo);

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	generate_vertices(vertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, axis_data.size() * sizeof(float), axis_data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);	// сделать активным VBO фигуры, чтобы можно было изменять детализацию

	shader = std::move(ShaderProgram(vertex_shader, fragment_shader));
	axis_shader = std::move(ShaderProgram(axis_vertex_shader, fragment_shader));
	shader.use();
	glm::mat4 rotate_matr = glm::rotate(glm::mat4(1.0f), glm::radians(-15.0f), glm::vec3(2.0, -1.0, 0.0));
	shader.set_uniform("rotate_matr", rotate_matr);
	shader.set_uniform("translate_matr", translate_matr);
	shader.set_uniform("scale_matr", scale_matr);
	axis_shader.set_uniform("rotate_matr", rotate_matr);
	axis_shader.set_uniform("scale_matr", scale_matr);
}


void draw()
{
	glBindVertexArray(vao[0]);
	shader.use();
	for (GLuint i = 0; i < circle_params.size(); i++)
	{
		glDrawArrays(GL_LINE_LOOP, i * vertices_per_circle, vertices_per_circle);
	}
	for (GLuint i = 0; i < vertical_count; i++)
	{
		glDrawArrays(GL_LINE_STRIP, (i + vertices_per_circle) * circle_params.size(), circle_params.size());
	}

	glBindVertexArray(vao[1]);
	axis_shader.use();
	glDrawArrays(GL_LINES, 0, 6);
}


void draw_deinit()
{
	glDeleteVertexArrays(2, vao);
	glDeleteBuffers(2, vbo);
}
