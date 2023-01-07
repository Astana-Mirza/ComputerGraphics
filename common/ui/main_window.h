#ifndef _CG_MAIN_WINDOW_H
#define _CG_MAIN_WINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>

class GLWindow;

class MainWindow : public Fl_Window
{
public:
	MainWindow(int width, int height, const char *title);

private:
	GLWindow *gl_window;
};


#endif // #ifndef _CG_MAIN_WINDOW_H