#include <common/utils.h>

#include <common/ui/main_window.h>
#include <common/ui/controls.h>
#include <controls.h>

int main()
{
	MainWindow<GLWindow> window(1050, 800, "Computer Graphics");

	spawn_background_control(window, 810, 10);
	spawn_translation_control(window, 810, 50);
	spawn_scale_control(window, 810, 240);
	spawn_rotation_control(window, 810, 430);
	spawn_detalization_control(window, 810, 620);
	window.get_gl_window()->set_draw_function(draw);
	window.show();
	draw_init();
	int ret = Fl::run();
	draw_deinit();
	return ret;
}
