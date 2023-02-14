#ifndef _CG_MAIN_WINDOW_H
#define _CG_MAIN_WINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>

#include <common/ui/gl_window.h>

#include <cassert>
#include <type_traits>

template <typename T>
class MainWindow : public Fl_Window
{

	static_assert(std::is_base_of<GLWindow, T>::value, "T must be GLWindow or its subclass");

public:
	MainWindow(int width, int height, const char *title);

	T* get_gl_window() const;

private:
	T *gl_window;
};


// implementation

template <typename T>
MainWindow<T>::MainWindow(int width, int height, const char* title) :
	Fl_Window(width, height, title), gl_window{ new T(800, 600) }
{
	add(gl_window);
	gl_window->show();
	end();
}


template <typename T>
T* MainWindow<T>::get_gl_window() const
{
	return gl_window;
}

#endif // #ifndef _CG_MAIN_WINDOW_H