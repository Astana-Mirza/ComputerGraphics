#ifndef _CG_LAB7_CALLBACKS_H
#define _CG_LAB7_CALLBACKS_H

class Fl_Widget;

void callback_select_projection(Fl_Widget* caller, void* data);
void callback_select_shape(Fl_Widget* caller, void* data);

// material settings
void callback_select_texture(Fl_Widget* caller, void* data);
void callback_select_diffuse(Fl_Widget* caller, void* data);
void callback_select_specular(Fl_Widget* caller, void* data);
void callback_select_shininess(Fl_Widget* caller, void* data);
void callback_select_amb_strength(Fl_Widget* caller, void* data);

// light settings
void callback_light_select_ambient(Fl_Widget* caller, void* data);
void callback_light_select_diffuse(Fl_Widget* caller, void* data);
void callback_light_select_specular(Fl_Widget* caller, void* data);
void callback_light_select_position_x(Fl_Widget* caller, void* data);
void callback_light_select_position_y(Fl_Widget* caller, void* data);
void callback_light_select_position_z(Fl_Widget* caller, void* data);
void callback_light_select_directional(Fl_Widget* caller, void* data);
void callback_light_select_direction_x(Fl_Widget* caller, void* data);
void callback_light_select_direction_y(Fl_Widget* caller, void* data);
void callback_light_select_direction_z(Fl_Widget* caller, void* data);
void callback_light_select_att_x(Fl_Widget* caller, void* data);
void callback_light_select_att_y(Fl_Widget* caller, void* data);
void callback_light_select_att_z(Fl_Widget* caller, void* data);
void callback_light_select_cut_off(Fl_Widget* caller, void* data);
void callback_light_select_outer_cut_off(Fl_Widget* caller, void* data);

#endif // #ifndef _CG_LAB7_CALLBACKS_H