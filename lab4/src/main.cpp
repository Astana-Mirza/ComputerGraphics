#include <common/utils.h>

#include <common/ui/main_window.h>
#include <common/ui/controls.h>
#include <controls.h>

int main()
{
	MainWindow<GLWindow> window(1050, 800, "Computer Graphics");

	spawn_background_control(window, 810, 10);
	spawn_knot_control(window, 810, 40);
	window.get_gl_window()->set_input_handle_function(handle);
	window.get_gl_window()->set_draw_function(draw);

	window.show();
	return Fl::run();
}
