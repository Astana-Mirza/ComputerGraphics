#include <controls.h>

#include <common/utils.h>
#include <Fl/Fl_Spinner.H>
#include <Fl/Fl_Check_Button.H>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

namespace // callbacks
{

static int fractal_iter_count = 2;
static const glm::vec4 fractal_start_point{ 0, 0, 0, 0 };
static constexpr float fractal_color[] = { 0.68f, 0.80f, 0.99f };
static bool draw_spiral = false;

static const glm::mat4 matrix30 = glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(0.0, 0.0, 1.0));
static const glm::mat4 matrix60 = glm::rotate(glm::mat4(1.0f), glm::radians(60.0f), glm::vec3(0.0, 0.0, 1.0));
static const glm::mat4 matrix_minus30 = glm::rotate(glm::mat4(1.0f), glm::radians(-30.0f), glm::vec3(0.0, 0.0, 1.0));
static const glm::mat4 matrix_minus60 = glm::rotate(glm::mat4(1.0f), glm::radians(-60.0f), glm::vec3(0.0, 0.0, 1.0));


void callback_count_control(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Spinner* spinner = static_cast<Fl_Spinner*>(caller);
	fractal_iter_count = static_cast<int>(spinner->value());
	window->get_gl_window()->redraw();
}


void callback_spiral_control(Fl_Widget* caller, void* data)
{
	draw_spiral = !draw_spiral;
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	window->get_gl_window()->redraw();
}


void spiral(glm::vec4 a, glm::vec4 b, glm::vec4 c, bool reverse = false)
{
	glm::vec4 dir1 = glm::normalize(b - a) * 0.025f;
	glm::vec4 dir2 = glm::normalize(c - b) * 0.02f;

	std::vector<glm::vec2> v[4];

	for (int i = 0; i < 8; i++)	// отрисовка четырёхугольников внутри основного, каждый поворачивается относительно предыдущего
	{
		dir1 = glm::rotate(glm::mat4(1.0f), glm::radians(-6.0f + i / 5.f), glm::vec3(0.0, 0.0, 1.0)) * dir1 * 1.1f;
		dir2 = glm::rotate(glm::mat4(1.0f), glm::radians(-6.0f + i / 5.f), glm::vec3(0.0, 0.0, 1.0)) * dir2 * 1.1f;

		a += dir1;
		b += dir2;
		glm::vec4 point_60_deg = a + (reverse ? matrix60 : matrix_minus60) * (b - a);
		glm::vec4 point_120_deg = point_60_deg + (reverse ? matrix_minus30 : matrix30) * ((b - a) / sqrt(3.0f));

		v[0].push_back({a.x, a.y});
		v[1].push_back({b.x, b.y});
		v[2].push_back({point_60_deg.x, point_60_deg.y});
		v[3].push_back({point_120_deg.x, point_120_deg.y});

		glColor3f(fractal_color[0] / 2, fractal_color[1] / 2, fractal_color[2] / 2);	// более тусклый цвет
		glBegin(GL_LINE_LOOP);
			glVertex2f(b.x, b.y);
			glVertex2f(a.x, a.y);
			glVertex2f(point_60_deg.x, point_60_deg.y);
			glVertex2f(point_120_deg.x, point_120_deg.y);
		glEnd();
	}

	glColor3f(fractal_color[0], fractal_color[1], fractal_color[2]);	// отрисовка обводки контура спирали
	for (int i = 0; i < 4; i++)
	{
		glBegin(GL_LINE_STRIP);
		for (const glm::vec2& point : v[i])
		{
			glVertex2f(point.x, point.y);
		}
		glEnd();
	}
}


void fractal(glm::vec2 start_point, glm::vec2 end_point, int depth, bool reverse = false)
{
	if (depth <= 0)
	{
		return;
	}

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
	if (draw_spiral && glm::length(end - start) > 0.3)
	{
		spiral({ start.x, start.y, 0, 0 }, { end.x, end.y, 0, 0 }, { point_60_deg.x, point_60_deg.y, 0, 0 }, reverse);
	}

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


void spawn_spiral_control(MainWindow<GLWindow>& window, int x, int y)
{
	Fl_Check_Button* button = new Fl_Check_Button(x, y, 220, 30, "Draw spirals");
	button->callback(callback_spiral_control, &window);
	window.get_scroll()->add(button);
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
