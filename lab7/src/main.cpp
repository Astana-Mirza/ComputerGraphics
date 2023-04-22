#include <common/utils.h>

#include <common/ui/main_window.h>
#include <common/ui/controls.h>
#include <controls.h>

int main()
{
	MainWindow<GLWindow> window(1050, 800, "Computer Graphics");

	spawn_background_control(window, 810, 10);
	spawn_projection_control(window, 810, 60);
	spawn_material_control(window, 810, 150);
	spawn_light_control(window, 810, 450);
	window.get_gl_window()->set_draw_function(draw);
	window.get_gl_window()->set_input_handle_function(handle);
	window.show();

	draw_init();
	Fl::add_timeout(1.0 / 60, timeout_callback, &window);
	int ret = Fl::run();
	draw_deinit();

	return ret;
}
