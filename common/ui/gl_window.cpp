#include <common/ui/gl_window.h>

#include <common/utils.h>

GLWindow::GLWindow(int width, int height, const char* title) :
	Fl_Gl_Window(width, height, title), draw_function{ nullptr },
	input_handle_function{ nullptr }, background_color { 0, 0, 0, 1 }
{
	mode(FL_RGB8 | FL_DOUBLE | FL_OPENGL3 | FL_ALPHA);
}


int GLWindow::handle(int event)
{
#if defined(USE_GLEW) && !defined(USE_OLD_OPENGL)
	static bool first = true;
	if (first && event == FL_SHOW && shown())
	{
		first = false;
		make_current();
		init_glew();
	}
#endif
	if (input_handle_function)
	{
		int ret = input_handle_function(event, *this);
		if (ret != 0)
		{
			return ret;
		}
	}
	return Fl_Gl_Window::handle(event);
}


void GLWindow::draw()
{
	if (!valid())
	{
		glViewport(0, 0, pixel_w(), pixel_h());
	}
	glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
	glClear(GL_COLOR_BUFFER_BIT);
	if (draw_function)
	{
		draw_function();
	}
}


void GLWindow::set_background_color(const Color& color)
{
	background_color = color;
}


void GLWindow::set_draw_function(DrawFunction func)
{
	draw_function = func;
}


void GLWindow::set_input_handle_function(HandleFunction func)
{
	input_handle_function = func;
}


const Color& GLWindow::get_background_color() const
{
	return background_color;
}
