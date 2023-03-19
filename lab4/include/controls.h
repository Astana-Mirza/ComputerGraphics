#ifndef _CG_LAB4_CONTROLS_H
#define _CG_LAB4_CONTROLS_H

#include <common/ui/main_window.h>
#include <common/ui/gl_window.h>

void draw();
int handle(int event, GLWindow& window);
void spawn_knot_control(MainWindow<GLWindow>& window, int x, int y);

#endif