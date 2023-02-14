#include <common/utils.h>

#include <common/ui/main_window.h>
#include <common/ui/controls.h>
#include <controls.h>

int main()
{
	MainWindow<GLWindow> window(1000, 600, "Computer Graphics");
	 
	spawn_background_control(window, 810, 10, 180, 50);
	spawn_primitive_control(window, 810, 70, 180, 50);
	window.get_gl_window()->set_draw_function(draw);

	window.show();
	return Fl::run();
}
