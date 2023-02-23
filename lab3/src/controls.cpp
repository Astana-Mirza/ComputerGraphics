#include <controls.h>

#include <common/utils.h>
#include <Fl/Fl_Spinner.H>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace // callbacks
{

static int fractal_iter_count = 2;
static const glm::vec4 fractal_start_point{ 0, 0, 0, 0 };
static constexpr float fractal_color[] = { 0.68f, 0.80f, 0.99f };

void callback_count_control(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Spinner* spinner = static_cast<Fl_Spinner*>(caller);
	fractal_iter_count = static_cast<int>(spinner->value());
	window->get_gl_window()->redraw();
}


void fractal(glm::vec2 start_point, glm::vec2 end_point, int depth, bool reverse = false)
{
	if (depth <= 0)
	{
		return;
	}
	static const glm::mat4 matrix30 = glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(0.0, 0.0, 1.0));
	static const glm::mat4 matrix60 = glm::rotate(glm::mat4(1.0f), glm::radians(60.0f), glm::vec3(0.0, 0.0, 1.0));
	static const glm::mat4 matrix_minus30 = glm::rotate(glm::mat4(1.0f), glm::radians(-30.0f), glm::vec3(0.0, 0.0, 1.0));
	static const glm::mat4 matrix_minus60 = glm::rotate(glm::mat4(1.0f), glm::radians(-60.0f), glm::vec3(0.0, 0.0, 1.0));

	glm::vec4 start{start_point, 0, 0};
	glm::vec4 end{ end_point, 0, 0};
	glm::vec4 point_60_deg = start + (reverse ? matrix60 : matrix_minus60) * (end - start);
	glm::vec4 point_120_deg = point_60_deg + (reverse ? matrix_minus30 : matrix30) * ((end - start) / sqrt(3.0f));


	glBegin(GL_LINE_LOOP);
		glVertex2f(end.x, end.y);
		glVertex2f(start.x, start.y);
		glVertex2f(point_60_deg.x, point_60_deg.y);
		glVertex2f(point_120_deg.x, point_120_deg.y);
	glEnd();

	fractal(start, point_60_deg, depth - 1, reverse);
	fractal(point_60_deg, point_120_deg, depth - 1, reverse);
	fractal(end, point_120_deg, depth - 1, !reverse);
}

} // callbacks


void spawn_count_control(MainWindow<GLWindow>& window, int x, int y)
{
	Fl_Spinner* spinner = new Fl_Spinner(x+150, y, 70, 30, "Depth of recursion  ");
	spinner->type(FL_INT_INPUT);
	spinner->minimum(1);
	spinner->maximum(10);
	spinner->value(2);
	spinner->callback(callback_count_control, &window);
	window.get_scroll()->add(spinner);
}


void draw()
{
	glColor3f(fractal_color[0], fractal_color[1], fractal_color[2]);
	fractal({0, 0}, {-0.5, 0}, fractal_iter_count);
	fractal({0, 0}, {-0.25, 0.433013}, fractal_iter_count);
	fractal({0, 0}, { 0.25, 0.433013 }, fractal_iter_count);
	fractal({0, 0}, {0.5, 0}, fractal_iter_count);
	fractal({0, 0}, {0.25, -0.433013}, fractal_iter_count);
	fractal({0, 0}, {-0.25, -0.433013}, fractal_iter_count);
}
