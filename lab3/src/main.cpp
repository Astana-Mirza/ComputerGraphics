﻿#include <common/utils.h>

#include <common/ui/main_window.h>
#include <common/ui/controls.h>
#include <controls.h>

int main()
{
	MainWindow<GLWindow> window(1050, 800, "Computer Graphics");

	spawn_background_control(window, 810, 10);
	spawn_count_control(window, 810, 50);
	spawn_spiral_control(window, 810, 90);
	window.get_gl_window()->set_draw_function(draw);

	window.show();
	return Fl::run();
}
