#ifndef _CG_LAB7_CONTROLS_H
#define _CG_LAB7_CONTROLS_H

#include <common/ui/main_window.h>
#include <common/ui/gl_window.h>

void spawn_material_control(MainWindow<GLWindow>& window, int x, int y);

int handle(int event, GLWindow& window);
void timeout_callback(void* arg);
void draw_init();
void draw();
void draw_deinit();

#endif