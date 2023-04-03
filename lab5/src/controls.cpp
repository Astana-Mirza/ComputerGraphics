#include <controls.h>

#include <common/utils.h>
#include <common/shader_program.h>

#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <FL/Fl_Hor_Slider.H>

#include <vector>
#include <chrono>

namespace // data
{

unsigned int vao, vbo;
const std::string vertex_shader = R"(
	#version 330 core
	layout (location = 0) in vec3 in_pos;
	out vec4 vertex_color;

	uniform float time;
	uniform float frequency;
	uniform float amplitude;
	uniform float wavelength;

	void main()
	{
		vec3 pos = in_pos;
		float value = sin(wavelength * pos.x - frequency * time);

		pos.y += value*amplitude;
		gl_Position = vec4(pos, 1.0);
		vertex_color = vec4(0.8, 1-abs(value), abs(value), 1.0);
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
ShaderProgram shader;

std::vector<glm::vec2> knots = {
	{ -0.9f,  -0.10f },
	{  0.0f,  -0.13f },
	{  0.9f,  -0.10f }
};

std::vector<glm::vec3> vertices;
std::chrono::milliseconds current_time;

} // data


namespace // functions
{

// сплайн Кочанека-Бартелса
void generate_vertices(const std::vector<glm::vec2>& knots, std::vector<glm::vec3>& vertices)
{
	vertices.clear();
	std::vector<std::pair<glm::vec2, glm::vec2>> derivatives(knots.size());
	for (int i = 1; i < knots.size() - 1; i++)			// расчёт значений производных
	{													// на основе параметров tension, bias, continuity
		std::pair<glm::vec2, glm::vec2> knot_der;
		glm::vec2 g1 = (knots[i] - knots[i - 1]) * 1.5f;
		glm::vec2 g2 = (knots[i + 1] - knots[i]) * 0.5f;
		glm::vec2 g3 = g2 - g1;
		knot_der.first  = 0.5f * (g1 + 0.75f * g3);
		knot_der.second = 0.5f * (g1 + 0.25f * g3);
		derivatives[i] = knot_der;
	}
	// обработка отдельного случая - для первой и последней точек
	derivatives[0].first = knots[1] - knots[0];
	derivatives[0].second = (1.5f * (knots[1] - knots[0]) - 0.5f * derivatives[1].first) * 0.5f;
	int i = knots.size() - 1;
	derivatives[i].first = (1.5f * (knots[i] - knots[i - 1]) - 0.5f * derivatives[i - 1].second) * 0.5f;
	derivatives[i].second = knots[i] - knots[i - 1];
	auto rotation_matr = glm::rotate(glm::mat4(1.0f), glm::radians(-15.0f), glm::vec3(2.0, -1.0, 0.0));

	for (int i = 0; i < knots.size() - 1; i++)
	{
		constexpr int point_count = 100;
		float step = 1.f / point_count;
		float t = 0.0f;
		for (int j = 0; j < point_count; j++)
		{
			glm::vec2 point = knots[i] * (2 * t * t * t - 3 * t * t * t + 1)
				+ derivatives[i].first * (t * t * t - 2 * t * t + t)
				+ knots[i + 1] * (-2 * t * t * t + 3 * t * t)
				+ derivatives[i + 1].second * (t * t * t - t * t);
			t += step;
			vertices.push_back(rotation_matr * glm::vec4{ point,  0.0f, 0.1f });
			vertices.push_back(rotation_matr * glm::vec4{ point, -1.0f, 0.1f });
		}
	}
}


void callback_select_amplitude(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Hor_Slider* slider = static_cast<Fl_Hor_Slider*>(caller);
	shader.set_uniform("amplitude", slider->value());
}


void callback_select_frequency(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Hor_Slider* slider = static_cast<Fl_Hor_Slider*>(caller);
	shader.set_uniform("frequency", slider->value());
}


void callback_select_wavelength(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Hor_Slider* slider = static_cast<Fl_Hor_Slider*>(caller);
	shader.set_uniform("wavelength", slider->value());
}


} // functions

void spawn_animation_control(MainWindow<GLWindow>& window, int x, int y)
{
	Fl_Hor_Slider* slider_amp = new Fl_Hor_Slider(x, y + 30, 220, 20, "Amplitude");
	Fl_Hor_Slider* slider_freq = new Fl_Hor_Slider(x, y + 80, 220, 20, "Frequency");
	Fl_Hor_Slider* slider_wavelength = new Fl_Hor_Slider(x, y + 130, 220, 20, "Wavelength");

	slider_amp->bounds(0.1, 0.6);
	slider_freq->bounds(0, 10);
	slider_wavelength->bounds(0, 10);
	slider_amp->callback(callback_select_amplitude, &window);
	slider_freq->callback(callback_select_frequency, &window);
	slider_wavelength->callback(callback_select_wavelength, &window);

	window.get_scroll()->add(slider_amp);
	window.get_scroll()->add(slider_freq);
	window.get_scroll()->add(slider_wavelength);
}


void timeout_callback(void* arg)
{
	current_time += std::chrono::milliseconds(1);
	static_cast<MainWindow<GLWindow>*>(arg)->get_gl_window()->redraw();
	Fl::repeat_timeout(1.0/60, timeout_callback, arg);
}


void draw_init()
{
	generate_vertices(knots, vertices);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	ShaderProgram main_shader(vertex_shader, fragment_shader);
	shader = std::move(main_shader);
	shader.set_uniform("amplitude", 0.1f);
	shader.set_uniform("frequency", 0.0f);
	shader.set_uniform("wavelength", 0.0f);
	shader.use();
}


void draw()
{
	using namespace std::chrono;
	shader.set_uniform("time", current_time.count()/50.f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
}


void draw_deinit()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}
