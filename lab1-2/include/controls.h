#ifndef _CG_LAB1_CONTROLS_H
#define _CG_LAB1_CONTROLS_H

#include <common/ui/main_window.h>
#include <common/ui/gl_window.h>

void spawn_primitive_control(MainWindow<GLWindow>& window, int x, int y);
void spawn_scissor_control(MainWindow<GLWindow>& window, int x, int y);
void spawn_alpha_control(MainWindow<GLWindow>& window, int x, int y);
void spawn_blend_control(MainWindow<GLWindow>& window, int x, int y);
void draw();

#endif