#ifndef _CG_GL_WINDOW_H
#define _CG_GL_WINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>

class GLWindow : public Fl_Gl_Window
{
public:
	GLWindow(int width, int height, const char *title = nullptr);
};

#endif // #ifndef _CG_GL_WINDOW_H