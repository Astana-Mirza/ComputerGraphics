#include <common/utils.h>

#include <common/ui/main_window.h>
#include <common/ui/controls.h>
#include <controls.h>

int main()
{
	MainWindow<GLWindow> window(1050, 600, "Computer Graphics");

	spawn_background_control(window, 810, 10);
	spawn_primitive_control(window, 810, 50);
	spawn_scissor_control(window, 810, 140);
	spawn_alpha_control(window, 810, 380);
	spawn_blend_control(window, 810, 520);
	window.get_gl_window()->set_draw_function(draw);

	window.show();
	return Fl::run();
}
