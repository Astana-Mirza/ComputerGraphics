#include <controls.h>

#include <common/utils.h>
#include <common/shader_program.h>
#include <common/camera.h>

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
#include <callbacks.h>

#include <cmath>
#include <chrono>

namespace movement
{

constexpr GLfloat speed = 0.1f;
const GLfloat sensitivity = 0.7f;

std::chrono::time_point last_update_time{std::chrono::steady_clock::now()};
glm::vec3 move_direction;
glm::vec2 prev_drag;

Camera camera;

} // namespace movement

static constexpr GLuint shape_count = 4;

GLuint ebo;
GLuint current_shape = 0;
std::vector<Shape> shapes;
ShaderProgram shader;
Light light;

#include <shape_shader.vs>
#include <shape_shader.fs>

void spawn_material_control(MainWindow<GLWindow>& window, int x, int y)
{
	Fl_Box* material_label = new Fl_Box(x, y, 220, 20, "Material settings");
	Fl_Spinner* spinner = new Fl_Spinner(x + 150, y + 30, 70, 30, "Shape number:   ");
	Fl_Button* select_button = new Fl_Button(x, y + 70, 220, 30, "Select texture");
	Fl_Button* diffuse_button = new Fl_Button(x, y + 110, 220, 30, "Diffuse and Ambient color");
	Fl_Button* specular_button = new Fl_Button(x, y + 150, 220, 30, "Specular color");
	Fl_Hor_Slider* shininess_slider = new Fl_Hor_Slider(x, y + 190, 220, 20, "Shininess");
	Fl_Hor_Slider* amb_strength_slider = new Fl_Hor_Slider(x, y + 230, 220, 20, "Ambient strength");

	material_label->labelfont(FL_BOLD);
	spinner->type(FL_INT_INPUT);
	spinner->minimum(1);
	spinner->maximum(shape_count);
	spinner->value(1);
	shininess_slider->value(128.0);
	shininess_slider->bounds(0, 256);
	amb_strength_slider->value(0.2);
	amb_strength_slider->bounds(0.0001, 1);

	spinner->callback(callback_select_shape, &window);
	select_button->callback(callback_select_texture, &window);
	diffuse_button->callback(callback_select_diffuse, &window);
	specular_button->callback(callback_select_specular, &window);
	shininess_slider->callback(callback_select_shininess, &window);
	amb_strength_slider->callback(callback_select_amb_strength, &window);

	window.get_scroll()->add(material_label);
	window.get_scroll()->add(select_button);
	window.get_scroll()->add(spinner);
	window.get_scroll()->add(diffuse_button);
	window.get_scroll()->add(specular_button);
	window.get_scroll()->add(shininess_slider);
	window.get_scroll()->add(amb_strength_slider);
}


void spawn_light_control(MainWindow<GLWindow>& window, int x, int y)
{
	Fl_Box* light_label = new Fl_Box(x, y, 220, 20, "Light settings");
	Fl_Button* ambient_button = new Fl_Button(x, y + 30, 220, 30, "Ambient color");
	Fl_Button* diffuse_button = new Fl_Button(x, y + 70, 220, 30, "Diffuse color");
	Fl_Button* specular_button = new Fl_Button(x, y + 110, 220, 30, "Specular color");
	Fl_Spinner* spinner_dir_x = new Fl_Spinner(x + 60, y + 150, 50, 30, "Direction:");
	Fl_Spinner* spinner_dir_y = new Fl_Spinner(x + 115, y + 150, 50, 30);
	Fl_Spinner* spinner_dir_z = new Fl_Spinner(x + 170, y + 150, 50, 30);
	Fl_Check_Button* check_direct = new Fl_Check_Button(x + 50, y + 190, 50, 30, "  Is Directional Light");
	Fl_Hor_Slider* spinner_pos_x = new Fl_Hor_Slider(x, y + 230, 220, 15, "Position X");
	Fl_Hor_Slider* spinner_pos_y = new Fl_Hor_Slider(x, y + 270, 220, 15, "Position Y");
	Fl_Hor_Slider* spinner_pos_z = new Fl_Hor_Slider(x, y + 310, 220, 15, "Position Z");
	Fl_Spinner* spinner_att_x = new Fl_Spinner(x + 60, y + 350, 50, 30, "Atten.: ");
	Fl_Spinner* spinner_att_y = new Fl_Spinner(x + 115, y + 350, 50, 30);
	Fl_Spinner* spinner_att_z = new Fl_Spinner(x + 170, y + 350, 50, 30);
	Fl_Hor_Slider* cut_off_slider = new Fl_Hor_Slider(x, y + 390, 220, 15, "Cut Offset");
	Fl_Hor_Slider* outer_cut_off_slider = new Fl_Hor_Slider(x, y + 430, 220, 15, "Outer Cut Offset");
	Fl_Box* padding_up = new Fl_Box(x, 0, 220, 1);
	Fl_Box* padding_down = new Fl_Box(x, y + 470, 220, 1);

	light_label->labelfont(FL_BOLD);
	spinner_dir_x->minimum(-1);          spinner_dir_x->maximum(1.0001);
	spinner_dir_y->minimum(-1);          spinner_dir_y->maximum(1.0001);
	spinner_dir_z->minimum(-1);          spinner_dir_z->maximum(1.0001);
	spinner_dir_x->type(FL_FLOAT_INPUT); spinner_dir_x->step(0.1);
	spinner_dir_y->type(FL_FLOAT_INPUT); spinner_dir_y->step(0.1);
	spinner_dir_z->type(FL_FLOAT_INPUT); spinner_dir_z->step(0.1);
	spinner_dir_x->value(light.direction.x);
	spinner_dir_y->value(light.direction.y);
	spinner_dir_z->value(light.direction.z);
	check_direct->value(light.position.w == 0.0f);
	spinner_pos_x->bounds(-1, 1);
	spinner_pos_y->bounds(-1, 1);
	spinner_pos_z->bounds(-1, 1);
	spinner_pos_x->value(light.position.x);
	spinner_pos_y->value(light.position.y);
	spinner_pos_z->value(light.position.z);
	spinner_att_x->minimum(0.5);        spinner_att_x->maximum(1.0001);
	spinner_att_y->minimum(0);          spinner_att_y->maximum(1.0001);
	spinner_att_z->minimum(0);          spinner_att_z->maximum(1.0001);
	spinner_att_x->type(FL_FLOAT_INPUT); spinner_att_x->step(0.001);
	spinner_att_y->type(FL_FLOAT_INPUT); spinner_att_y->step(0.001);
	spinner_att_z->type(FL_FLOAT_INPUT); spinner_att_z->step(0.001);
	spinner_att_x->value(light.atten_params.x);
	spinner_att_y->value(light.atten_params.y);
	spinner_att_z->value(light.atten_params.z);
	cut_off_slider->value(0);
	cut_off_slider->bounds(0, 1);
	outer_cut_off_slider->value(0);
	outer_cut_off_slider->bounds(0, 1);

	ambient_button->callback(callback_light_select_ambient, &window);
	diffuse_button->callback(callback_light_select_diffuse, &window);
	specular_button->callback(callback_light_select_specular, &window);
	spinner_dir_x->callback(callback_light_select_direction_x, &window);
	spinner_dir_y->callback(callback_light_select_direction_y, &window);
	spinner_dir_z->callback(callback_light_select_direction_z, &window);
	check_direct->callback(callback_light_select_directional, &window);
	spinner_pos_x->callback(callback_light_select_position_x, &window);
	spinner_pos_y->callback(callback_light_select_position_y, &window);
	spinner_pos_z->callback(callback_light_select_position_z, &window);
	spinner_att_x->callback(callback_light_select_att_x, &window);
	spinner_att_y->callback(callback_light_select_att_y, &window);
	spinner_att_z->callback(callback_light_select_att_z, &window);
	cut_off_slider->callback(callback_light_select_cut_off, &window);
	outer_cut_off_slider->callback(callback_light_select_outer_cut_off, &window);

	window.get_scroll()->add(light_label);
	window.get_scroll()->add(ambient_button);
	window.get_scroll()->add(diffuse_button);
	window.get_scroll()->add(specular_button);
	window.get_scroll()->add(spinner_dir_x);
	window.get_scroll()->add(spinner_dir_y);
	window.get_scroll()->add(spinner_dir_z);
	window.get_scroll()->add(check_direct);
	window.get_scroll()->add(spinner_pos_x);
	window.get_scroll()->add(spinner_pos_y);
	window.get_scroll()->add(spinner_pos_z);
	window.get_scroll()->add(spinner_att_x);
	window.get_scroll()->add(spinner_att_y);
	window.get_scroll()->add(spinner_att_z);
	window.get_scroll()->add(cut_off_slider);
	window.get_scroll()->add(outer_cut_off_slider);
	window.get_scroll()->add(padding_up);
	window.get_scroll()->add(padding_down);
}


void spawn_projection_control(MainWindow<GLWindow>& window, int x, int y)
{
	Fl_Box* projection_label = new Fl_Box(x, y, 220, 20, "Projection");
	Fl_Menu_Button* choice = new Fl_Menu_Button(x, y + 30, 220, 30, "Perspective");
	projection_label->labelfont(FL_BOLD);
	choice->add("Perspective");
	choice->add("Orthographic");
	choice->callback(callback_select_projection, &window);
	window.get_scroll()->add(projection_label);
	window.get_scroll()->add(choice);
}


int handle(int event, GLWindow& window)
{
	int ret = 0;
	switch (event)
	{
		case FL_PUSH:
			window.take_focus();
			movement::prev_drag = glm::vec2{
				   static_cast<float>(Fl::event_x()) / window.pixel_w() * 2 - 1,
				   static_cast<float>(Fl::event_y()) / window.pixel_w() * -2 + 1
			};
		case FL_FOCUS:
		case FL_UNFOCUS:
		case FL_RELEASE:
		{
			return 1;
		}
		break;
		case FL_KEYDOWN:
		{
			switch (Fl::event_key())
			{
				case 'w': movement::move_direction.y =  1; break;
				case 'a': movement::move_direction.x = -1; break;
				case 's': movement::move_direction.y = -1; break;
				case 'd': movement::move_direction.x =  1; break;
				case 'e': movement::move_direction.z = 1; break;
				case 'q': movement::move_direction.z = -1; break;
				default: return 0;
			}
			ret = 1;
		}
		break;
		case FL_KEYUP:
		{
			switch (Fl::event_key())
			{
				case 'w': case 's': movement::move_direction.y = 0; break;
				case 'a': case 'd': movement::move_direction.x = 0; break;
				case 'e': case 'q': movement::move_direction.z = 0; break;
				default: return 0;
			}
			ret = 1;
		}
		break;
		case FL_DRAG:
		{
			glm::vec2 new_drag{
				static_cast<float>(Fl::event_x()) / window.pixel_w() * 2 - 1,
				static_cast<float>(Fl::event_y()) / window.pixel_w() * -2 + 1
			};
			glm::vec3 rot{ (new_drag - movement::prev_drag) * movement::sensitivity, 0.0f };
			movement::camera.rotate({ rot.y, rot.x, 0.0f });
			movement::prev_drag = new_drag;
			ret = 1;
		}
		break;
		default:
			return 0;
	}
	return ret;
}


void timeout_callback(void* arg)
{
	auto now = std::chrono::steady_clock::now();
	GLfloat delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - movement::last_update_time).count() / 1000.f;
	if (movement::move_direction.x != 0 || movement::move_direction.y != 0 || movement::move_direction.z != 0)
	{
		const glm::vec3& dir = movement::camera.get_direction();
		const glm::vec3& right = movement::camera.get_right();
		const glm::vec3& up = movement::camera.get_up();
		movement::camera.move((dir * movement::move_direction.y
							 + right * movement::move_direction.x
							 + up * movement::move_direction.z) * movement::speed);
	}
	shader.set_uniform("view", movement::camera.get_look_at_matr());
	shader.set_uniform("camera_pos", movement::camera.get_position());
	static_cast<MainWindow<GLWindow>*>(arg)->get_gl_window()->redraw();
	Fl::repeat_timeout(1.0 / 60, timeout_callback, arg);
	movement::last_update_time = now;
}


void draw_init()
{
	static constexpr GLfloat a[] = { 0.5f, 0.2f, 0.8f, 0.0f };
	static constexpr GLfloat b[] = { 0.4f, 0.0f, 1.0f, 0.0f };
	static constexpr GLfloat c[] = { 0.1f, 0.1f, 0.6f, 1.0f };
	static constexpr GLfloat d[] = { 3.0f, 2.0f, 5.0f, 1.0f };
	static const glm::mat4 models[] = {
		{0.5f, 0.0f, 0.0f, 0.0f,
		 0.0f, 0.5f, 0.0f, 0.0f,
		 0.0f, 0.0f, 0.5f, 0.0f,
		 0.5f, 0.5f, 0.5f, 1.0f},

		glm::rotate(glm::mat4{
		 0.5f, 0.0f, 0.0f, 0.0f,
		 0.0f, 0.5f, 0.0f, 0.0f,
		 0.0f, 0.0f, 0.5f, 0.0f,
		-0.7f, 0.0f, 0.2f, 1.0f},
		glm::radians(60.0f), {1.0f, 0.0f, 0.2f}),

		{0.1f, 0.0f, 0.0f, 0.0f,
		 0.0f, 0.5f, 0.0f, 0.0f,
		 0.0f, 0.0f, 0.1f, 0.0f,
		-0.2f, 0.3f, 0.5f, 1.0f},

		{0.3f, 0.0f, 0.0f, 0.0f,
		 0.0f, 0.4f, 0.0f, 0.0f,
		 0.0f, 0.0f, 0.3f, 0.0f,
		 0.0f, 0.1f, -0.7f, 1.0f}
	};

	GLuint vao[shape_count];
	GLuint vbo[shape_count];
	GLuint texture[shape_count];
	std::vector<GLuint> indices = Shape::generate_indices();

	glGenBuffers(1, &ebo);				// общий EBO для всех фигур
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(shape_count, vao);
	glGenBuffers(shape_count, vbo);
	glGenTextures(shape_count, texture);

	for (GLuint i = 0; i < shape_count; i++)
	{
		Shape shape;
		shape.model_transform = models[i];
		shape.vao = vao[i];
		shape.ebo = ebo;
		shape.vbo = vbo[i];
		shape.texture = texture[i];
		shape.index_count = static_cast<GLuint>(indices.size());

		shape.generate_vertices(a[i], b[i], c[i], d[i]);
		shape.calculate_normal_matr();
		shape.load_texture("bricks.png");
		shapes.push_back(shape);
	}

	shader = std::move(ShaderProgram(vertex_shader, fragment_shader));
	shader.use();
	shader.set_uniform("tex", 0);
	shader.set_uniform("projection", glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f));
	shader.set_uniform("view", movement::camera.get_look_at_matr());
	shader.set_uniform("camera_pos", movement::camera.get_position());

	light.apply_settings(shader);
	glEnable(GL_DEPTH_TEST);
}


void draw()
{
	for (Shape& shape : shapes)
	{
		shape.draw(shader);
	}
}


void draw_deinit()
{
	glDeleteBuffers(1, &ebo);
	for (Shape& shape : shapes)
	{
		glDeleteVertexArrays(1, &shape.vao);
		glDeleteBuffers(1, &shape.vbo);
		glDeleteTextures(1, &shape.texture);
	}
}
