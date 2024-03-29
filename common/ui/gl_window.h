#ifndef _CG_GL_WINDOW_H
#define _CG_GL_WINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>

#include <common/color.h>

class GLWindow : public Fl_Gl_Window
{
public:
	using DrawFunction = void(*)();
	using HandleFunction = int(*)(int, GLWindow&);

	GLWindow(int width, int height, const char *title = nullptr);

	int handle(int event);
	void set_background_color(const Color& color);
	void set_draw_function(DrawFunction func);
	void set_input_handle_function(HandleFunction func);

	const Color& get_background_color() const;

protected:
	void draw();

private:
	DrawFunction draw_function;
	HandleFunction input_handle_function;
	Color background_color;
};

#endif // #ifndef _CG_GL_WINDOW_H