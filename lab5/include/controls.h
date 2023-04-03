#ifndef _CG_LAB5_CONTROLS_H
#define _CG_LAB5_CONTROLS_H

#include <common/ui/main_window.h>
#include <common/ui/gl_window.h>

void spawn_animation_control(MainWindow<GLWindow>& window, int x, int y);
void timeout_callback(void* arg);
void draw_init();
void draw();
void draw_deinit();

#endif