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

#include <vertex.h>
#include <shape.h>

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

namespace // data
{

static constexpr GLuint shape_count = 1;

GLuint vao;
GLuint ebo;
GLuint current_shape = 0;
std::vector<Shape> shapes;
ShaderProgram shader;

const std::string vertex_shader = R"(
	#version 330 core
	layout (location = 0) in vec3 in_pos;
	layout (location = 1) in vec3 in_normal;
	layout (location = 2) in vec2 in_texture_pos;

	out vec3 normal;
	out vec2 texture_pos;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(in_pos, 1.0);
		normal = in_normal;
		texture_pos = in_texture_pos;
	}
)";

const std::string fragment_shader = R"(
	#version 330 core
	in vec3 normal;
	in vec2 texture_pos;

	out vec4 out_color;

	uniform sampler2D tex;

	void main()
	{
	    out_color = texture(tex, texture_pos);
	} 
)";

} // data


namespace // functions
{

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
	current_shape = spinner->value() - 1;
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

} // functions


void spawn_material_control(MainWindow<GLWindow>& window, int x, int y)
{
	Fl_Box* material_label = new Fl_Box(x, y, 220, 20, "Material settings");
	Fl_Spinner* spinner = new Fl_Spinner(x + 150, y + 30, 70, 30, "Shape number:   ");
	Fl_Button* select_button = new Fl_Button(x, y + 80, 220, 30, "Select texture");

	material_label->labelfont(FL_BOLD);
	spinner->type(FL_INT_INPUT);
	spinner->minimum(1);
	spinner->maximum(shape_count);
	spinner->value(1);
	spinner->callback(callback_select_shape, &window);
	select_button->callback(callback_select_texture, &window);

	window.get_scroll()->add(select_button);
	window.get_scroll()->add(material_label);
	window.get_scroll()->add(spinner);
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
	static_cast<MainWindow<GLWindow>*>(arg)->get_gl_window()->redraw();
	Fl::repeat_timeout(1.0 / 60, timeout_callback, arg);
	movement::last_update_time = now;
}


void draw_init()
{
	static constexpr GLfloat a[] = { 0.5f, 1.0f, 1.0f, 1.0f };
	static constexpr GLfloat b[] = { 0.25f, 1.0f, 1.0f, 1.0f };
	static constexpr GLfloat c[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	static constexpr GLfloat d[] = { 0.09f, 0.0f, 0.0f, 0.0f };

	GLuint vbo[shape_count];
	GLuint texture[shape_count];

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &ebo);
	glGenBuffers(shape_count, vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	std::vector<GLuint> indices = Shape::generate_indices();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	glGenTextures(shape_count, texture);

	for (GLuint i = 0; i < shape_count; i++)
	{
		Shape shape;
		shape.vao = vao;
		shape.ebo = ebo;
		shape.vbo = vbo[i];
		shape.texture = texture[i];
		shape.index_count = static_cast<GLuint>(indices.size());
		shape.generate_vertices(a[i], b[i], c[i], d[i]);
		shape.load_texture();
		shapes.push_back(shape);
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);  // position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));  // normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(glm::vec3)));  // texture_coords
	glEnableVertexAttribArray(2);

	shader = std::move(ShaderProgram(vertex_shader, fragment_shader));
	shader.use();
	shader.set_uniform("tex", 0);
	shader.set_uniform("projection", glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f));
	shader.set_uniform("view", movement::camera.get_look_at_matr());
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
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &ebo);
	for (Shape& shape : shapes)
	{
		glDeleteBuffers(1, &shape.vbo);
		glDeleteTextures(1, &shape.texture);
	}
}
