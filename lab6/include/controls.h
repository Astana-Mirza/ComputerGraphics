#ifndef _CG_LAB6_CONTROLS_H
#define _CG_LAB6_CONTROLS_H

#include <common/ui/main_window.h>
#include <common/ui/gl_window.h>

void spawn_translation_control(MainWindow<GLWindow>& window, int x, int y);
void spawn_scale_control(MainWindow<GLWindow>& window, int x, int y);
void spawn_rotation_control(MainWindow<GLWindow>& window, int x, int y);
void spawn_detalization_control(MainWindow<GLWindow>& window, int x, int y);
void draw_init();
void draw();
void draw_deinit();

#endif