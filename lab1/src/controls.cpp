#include <controls.h>

#include <common/utils.h>
#include <Fl/Fl_Menu_Button.H>

#include <iostream>

namespace
{

static GLenum current_mode = GL_POINTS;
static constexpr float points[] = {
	-0.84, 0.26,
	-0.72, 0.74,
	-0.38, 0.78,
	0.28, 0.64,
	0.76, 0.40,
	0.86, 0.06,
	0.82, -0.32,
	0.62, -0.78,
	0.08, -0.90,
	-0.58, -0.74,
	-0.82, -0.42,
	-0.88, -0.18
};

static constexpr float colors[] = {
	0.32, 0.58, 0.15,
	0.52, 0.95, 0.43,
	0.03, 0.89, 0.77,
	0.12, 0.12, 0.63,
	0.67, 0.81, 0.73,
	0.36, 0.77, 0.96,
	0.57, 0.05, 0.62,
	0.25, 0.82, 0.77,
	0.92, 0.09, 0.43,
	0.33, 0.67, 0.57,
	0.88, 0.17, 0.79,
	0.22, 0.91, 0.02
};


void callback_select_primitive(Fl_Widget* caller, void* data)
{
	static constexpr GLenum modes[] = {
		  GL_POINTS
		, GL_LINES
		, GL_LINE_STRIP
		, GL_LINE_LOOP
		, GL_TRIANGLES
		, GL_TRIANGLE_STRIP
		, GL_TRIANGLE_FAN
		// удалены в OpenGL >=3.0, нужно использовать только с glBegin()/glEnd()
		, GL_QUADS
		, GL_QUAD_STRIP
		, GL_POLYGON
	};

	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Menu_Button* choice = static_cast<Fl_Menu_Button*>(caller);
	auto gl_subwindow = window->get_gl_window();
	current_mode = modes[choice->value()];
	choice->label(choice->text());
	gl_subwindow->redraw();
}

}

void spawn_primitive_control(MainWindow<GLWindow>& window, int x, int y, int w, int h)
{
	Fl_Menu_Button* choice = new Fl_Menu_Button(x, y, w, h, "GL_POINTS");
	choice->add("GL_POINTS");
	choice->add("GL_LINES");
	choice->add("GL_LINE_STRIP");
	choice->add("GL_LINE_LOOP");
	choice->add("GL_TRIANGLES");
	choice->add("GL_TRIANGLE_STRIP");
	choice->add("GL_TRIANGLE_FAN");
	choice->add("GL_QUADS");
	choice->add("GL_QUAD_STRIP");
	choice->add("GL_POLYGON");
	choice->callback(callback_select_primitive, &window);
	window.add(choice);
}


void draw()
{
	glBegin(current_mode);
		for (size_t i = 0; i < 12; i++)
		{
			glColor3f(colors[3*i], colors[3*i+1], colors[3*i+2]);
			glVertex2f(points[2*i], points[2*i+1]);
		}
	glEnd();

}
