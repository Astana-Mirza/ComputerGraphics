#include <common/ui/controls.h>

#include <FL/Fl_Button.H>
#include <FL/Fl_Color_Chooser.H> 

namespace
{

void callback_select_color(Fl_Widget *caller, void *data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	auto gl_subwindow = window->get_gl_window();
	Color current_color = gl_subwindow->get_background_color();
	double r = current_color.r;
	double g = current_color.g;
	double b = current_color.b;
	if (fl_color_chooser("Select background color", r, g, b))
	{
		gl_subwindow->set_background_color({
			static_cast<float>(r),
			static_cast<float>(g),
			static_cast<float>(b)
		});
		gl_subwindow->redraw();
	}
}

}


void spawn_background_control(MainWindow<GLWindow>& window, int x, int y)
{
	Fl_Button* select_button = new Fl_Button(x, y, 220, 30, "Background color");
	select_button->callback(callback_select_color, &window);
	window.get_scroll()->add(select_button);
}
