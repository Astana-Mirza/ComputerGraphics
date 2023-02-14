#ifndef _CG_LAB1_CONTROLS_H
#define _CG_LAB1_CONTROLS_H

#include <common/ui/main_window.h>
#include <common/ui/gl_window.h>

void spawn_primitive_control(MainWindow<GLWindow>& window, int x, int y, int w, int h);
void draw();

#endif