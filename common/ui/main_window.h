#ifndef _CG_MAIN_WINDOW_H
#define _CG_MAIN_WINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Scroll.H>

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
	Fl_Scroll* get_scroll() const;

private:
	T *gl_window;
	Fl_Scroll *scroll;
};


// implementation

template <typename T>
MainWindow<T>::MainWindow(int width, int height, const char* title) :
	Fl_Window(width, height, title), gl_window{ new T(width - 250, height) }, scroll{ new Fl_Scroll(width-250, 0, 250, height) }
{
	add(gl_window);
	add(scroll);
	scroll->type(Fl_Scroll::VERTICAL);
	scroll->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
	gl_window->show();
	end();
}


template <typename T>
T* MainWindow<T>::get_gl_window() const
{
	return gl_window;
}


template <typename T>
Fl_Scroll* MainWindow<T>::get_scroll() const
{
	return scroll;
}

#endif // #ifndef _CG_MAIN_WINDOW_H