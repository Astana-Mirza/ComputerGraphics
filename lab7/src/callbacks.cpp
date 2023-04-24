#include <callbacks.h>

#include <common/utils.h>
#include <common/shader_program.h>
#include <common/camera.h>
#include <common/ui/main_window.h>
#include <common/ui/gl_window.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <FL/Fl_Hor_Slider.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Spinner.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <Fl/Fl_Menu_Button.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Color_Chooser.H>

#include <shape.h>
#include <light.h>

#include <vector>

extern GLuint current_shape;
extern std::vector<Shape> shapes;
extern ShaderProgram shader;
extern Light light;


void callback_select_projection(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Menu_Button* choice = static_cast<Fl_Menu_Button*>(caller);
	if (choice->value())
	{
		shader.set_uniform("projection", glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 100.0f));
	}
	else
	{
		shader.set_uniform("projection", glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f));
	}
	choice->label(choice->text());
}


void callback_select_shape(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Spinner* spinner = static_cast<Fl_Spinner*>(caller);
	current_shape = static_cast<GLuint>(spinner->value()) - 1;
}


void callback_select_texture(Fl_Widget* caller, void* data)
{
	Fl_Native_File_Chooser chooser;
	chooser.title("Select texture image");
	chooser.type(Fl_Native_File_Chooser::BROWSE_FILE);
	chooser.filter("PNG image\t*.png\n");

	int ret = chooser.show();
	if (ret != 1 && ret != -1)
	{
		shapes[current_shape].load_texture(chooser.filename());
	}
	//	else if (ret == 1)
	//	{
	//		shapes[current_shape].load_texture();
	//	}
}


void callback_select_diffuse(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	auto gl_subwindow = window->get_gl_window();
	double r = shapes[current_shape].diffuse.x;
	double g = shapes[current_shape].diffuse.y;
	double b = shapes[current_shape].diffuse.z;
	if (fl_color_chooser("Select diffuse color", r, g, b))
	{
		shapes[current_shape].diffuse = { static_cast<float>(r), static_cast<float>(g), static_cast<float>(b) };
	}
}


void callback_select_specular(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	auto gl_subwindow = window->get_gl_window();
	double r = shapes[current_shape].specular.x;
	double g = shapes[current_shape].specular.y;
	double b = shapes[current_shape].specular.z;
	if (fl_color_chooser("Select specular color", r, g, b))
	{
		shapes[current_shape].specular = { static_cast<float>(r), static_cast<float>(g), static_cast<float>(b) };
	}
}


void callback_select_shininess(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Hor_Slider* slider = static_cast<Fl_Hor_Slider*>(caller);
	shapes[current_shape].shininess = static_cast<GLfloat>(slider->value());

}


void callback_select_amb_strength(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Hor_Slider* slider = static_cast<Fl_Hor_Slider*>(caller);
	shapes[current_shape].amb_strength = static_cast<GLfloat>(slider->value());
}


void callback_light_select_ambient(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	auto gl_subwindow = window->get_gl_window();
	double r = light.ambient.x;
	double g = light.ambient.y;
	double b = light.ambient.z;
	if (fl_color_chooser("Select light ambient color", r, g, b))
	{
		light.ambient = { static_cast<float>(r), static_cast<float>(g), static_cast<float>(b) };
		light.apply_settings(shader);
	}
}


void callback_light_select_diffuse(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	auto gl_subwindow = window->get_gl_window();
	double r = light.diffuse.x;
	double g = light.diffuse.y;
	double b = light.diffuse.z;
	if (fl_color_chooser("Select light diffuse color", r, g, b))
	{
		light.diffuse = { static_cast<float>(r), static_cast<float>(g), static_cast<float>(b) };
		light.apply_settings(shader);
	}
}


void callback_light_select_specular(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	auto gl_subwindow = window->get_gl_window();
	double r = light.specular.x;
	double g = light.specular.y;
	double b = light.specular.z;
	if (fl_color_chooser("Select light specular color", r, g, b))
	{
		light.specular = { static_cast<float>(r), static_cast<float>(g), static_cast<float>(b) };
		light.apply_settings(shader);
	}
}


void callback_light_select_position_x(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Hor_Slider* spinner = static_cast<Fl_Hor_Slider*>(caller);
	light.position.x = static_cast<GLfloat>(spinner->value());
	light.apply_settings(shader);
}


void callback_light_select_position_y(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Hor_Slider* spinner = static_cast<Fl_Hor_Slider*>(caller);
	light.position.y = static_cast<GLfloat>(spinner->value());
	light.apply_settings(shader);
}


void callback_light_select_position_z(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Hor_Slider* spinner = static_cast<Fl_Hor_Slider*>(caller);
	light.position.z = static_cast<GLfloat>(spinner->value());
	light.apply_settings(shader);
}


void callback_light_select_directional(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Check_Button* check = static_cast<Fl_Check_Button*>(caller);
	light.position.w = static_cast<GLfloat>(!check->value());
	light.apply_settings(shader);
}


void callback_light_select_direction_x(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Spinner* spinner = static_cast<Fl_Spinner*>(caller);
	light.direction.x = static_cast<GLfloat>(spinner->value());
	light.apply_settings(shader);
}


void callback_light_select_direction_y(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Spinner* spinner = static_cast<Fl_Spinner*>(caller);
	light.direction.y = static_cast<GLfloat>(spinner->value());
	light.apply_settings(shader);
}


void callback_light_select_direction_z(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Spinner* spinner = static_cast<Fl_Spinner*>(caller);
	light.direction.z = static_cast<GLfloat>(spinner->value());
	light.apply_settings(shader);
}


void callback_light_select_att_x(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Spinner* spinner = static_cast<Fl_Spinner*>(caller);
	light.atten_params.y = static_cast<GLfloat>(spinner->value());
	light.apply_settings(shader);
}


void callback_light_select_att_y(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Spinner* spinner = static_cast<Fl_Spinner*>(caller);
	light.atten_params.y = static_cast<GLfloat>(spinner->value());
	light.apply_settings(shader);
}


void callback_light_select_att_z(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Spinner* spinner = static_cast<Fl_Spinner*>(caller);
	light.atten_params.y = static_cast<GLfloat>(spinner->value());
	light.apply_settings(shader);
}


void callback_light_select_cut_off(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Hor_Slider* spinner = static_cast<Fl_Hor_Slider*>(caller);
	light.cut_off = static_cast<GLfloat>(spinner->value());
	light.apply_settings(shader);
}


void callback_light_select_outer_cut_off(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Hor_Slider* spinner = static_cast<Fl_Hor_Slider*>(caller);
	light.outer_cut_off = static_cast<GLfloat>(spinner->value());
	light.apply_settings(shader);
}
