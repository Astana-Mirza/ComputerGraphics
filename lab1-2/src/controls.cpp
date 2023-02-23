#include <controls.h>

#include <common/utils.h>
#include <Fl/Fl_Menu_Button.H>
#include <FL/Fl_Hor_Slider.H>
#include <FL/Fl_Box.H>

namespace // callbacks
{

// параметры отрисовки
static GLenum current_mode = GL_POINTS;
static GLint scissor_params[] = { 0, 0, 800, 600 };
static GLenum current_alpha_test = GL_ALWAYS;
static GLenum current_blend_src = GL_ONE;
static GLenum current_blend_dst = GL_ZERO;
static GLfloat alpha_test_ref = 0.0f;

// точки для отрисовки
static constexpr float points[] = {
	-0.84f,  0.26f,
	-0.72f,  0.74f,
	-0.38f,  0.78f,
	 0.28f,  0.64f,
	 0.76f,  0.40f,
	 0.86f,  0.06f,
	 0.82f, -0.32f,
	 0.62f, -0.78f,
	 0.08f, -0.90f,
	-0.58f, -0.74f,
	-0.82f, -0.42f,
	-0.88f, -0.18f
};

// цвета отрисовки
static constexpr float colors[] = {
	0.32f, 0.58f, 0.15f, 0.09f,
	0.52f, 0.95f, 0.43f, 0.12f,
	0.03f, 0.89f, 0.77f, 0.24f,
	0.12f, 0.12f, 0.63f, 0.30f,
	0.67f, 0.81f, 0.73f, 0.35f,
	0.36f, 0.77f, 0.96f, 0.46f,
	0.57f, 0.05f, 0.62f, 0.55f,
	0.25f, 0.82f, 0.77f, 0.68f,
	0.92f, 0.09f, 0.43f, 0.77f,
	0.33f, 0.67f, 0.57f, 0.83f,
	0.88f, 0.17f, 0.79f, 0.92f,
	0.22f, 0.91f, 0.02f, 1.00f
};


void callback_select_primitive(Fl_Widget* caller, void* data)
{
	static constexpr GLenum modes[] = {
		  GL_POINTS
		, GL_LINES
		, GL_LINE_STRIP
		, GL_LINE_LOOP
		, GL_TRIANGLES
		, GL_TRIANGLE_STRIP
		, GL_TRIANGLE_FAN
		// удалены в OpenGL >=3.0, нужно использовать только с glBegin()/glEnd()
		, GL_QUADS
		, GL_QUAD_STRIP
		, GL_POLYGON
	};

	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Menu_Button* choice = static_cast<Fl_Menu_Button*>(caller);
	auto gl_subwindow = window->get_gl_window();
	current_mode = modes[choice->value()];
	choice->label(choice->text());
	gl_subwindow->redraw();
}


template <size_t N>
void callback_select_scissor(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Hor_Slider* slider = static_cast<Fl_Hor_Slider*>(caller);
	scissor_params[N] = static_cast<GLint>(slider->value());
	window->get_gl_window()->redraw();
}


void callback_select_alpha_test(Fl_Widget* caller, void* data)
{
	static constexpr GLenum modes[] = {
		  GL_ALWAYS
		, GL_NEVER
		, GL_EQUAL
		, GL_NOTEQUAL
		, GL_LESS
		, GL_LEQUAL
		, GL_GREATER
		, GL_GEQUAL
	};

	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Menu_Button* choice = static_cast<Fl_Menu_Button*>(caller);
	auto gl_subwindow = window->get_gl_window();
	current_alpha_test = modes[choice->value()];
	choice->label(choice->text());
	gl_subwindow->redraw();
}


void callback_select_alpha_ref(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Hor_Slider* slider = static_cast<Fl_Hor_Slider*>(caller);
	alpha_test_ref = static_cast<GLfloat>(slider->value());
	window->get_gl_window()->redraw();
}


void callback_select_blend_src(Fl_Widget* caller, void* data)
{
	static constexpr GLenum modes[] = {
			  GL_ZERO
			, GL_ONE
			, GL_DST_COLOR
			, GL_ONE_MINUS_DST_COLOR
			, GL_SRC_ALPHA
			, GL_ONE_MINUS_SRC_ALPHA
			, GL_DST_ALPHA
			, GL_ONE_MINUS_DST_ALPHA
			, GL_SRC_ALPHA_SATURATE
	};

	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Menu_Button* choice = static_cast<Fl_Menu_Button*>(caller);
	auto gl_subwindow = window->get_gl_window();
	current_blend_src = modes[choice->value()];
	choice->label(choice->text());
	gl_subwindow->redraw();
}


void callback_select_blend_dst(Fl_Widget* caller, void* data)
{
	static constexpr GLenum modes[] = {
			  GL_ZERO
			, GL_ONE
			, GL_SRC_COLOR
			, GL_ONE_MINUS_SRC_COLOR
			, GL_SRC_ALPHA
			, GL_ONE_MINUS_SRC_ALPHA
			, GL_DST_ALPHA
			, GL_ONE_MINUS_DST_ALPHA
	};

	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Menu_Button* choice = static_cast<Fl_Menu_Button*>(caller);
	auto gl_subwindow = window->get_gl_window();
	current_blend_dst = modes[choice->value()];
	choice->label(choice->text());
	gl_subwindow->redraw();
}

} // callbacks

void spawn_primitive_control(MainWindow<GLWindow>& window, int x, int y)
{
	Fl_Box* label = new Fl_Box(x, y, 220, 30, "Primitive type");
	Fl_Menu_Button* choice = new Fl_Menu_Button(x, y+30, 220, 30, "GL_POINTS");

	label->labelfont(FL_BOLD);
	choice->add("GL_POINTS");
	choice->add("GL_LINES");
	choice->add("GL_LINE_STRIP");
	choice->add("GL_LINE_LOOP");
	choice->add("GL_TRIANGLES");
	choice->add("GL_TRIANGLE_STRIP");
	choice->add("GL_TRIANGLE_FAN");
	choice->add("GL_QUADS");
	choice->add("GL_QUAD_STRIP");
	choice->add("GL_POLYGON");
	choice->callback(callback_select_primitive, &window);

	window.get_scroll()->add(label);
	window.get_scroll()->add(choice);
}


void spawn_scissor_control(MainWindow<GLWindow>& window, int x, int y)
{
	Fl_Box* label = new Fl_Box(x, y, 220, 30, "Scissor test");
	Fl_Hor_Slider* slider_x = new Fl_Hor_Slider(x, y+30, 220, 20, "X");
	Fl_Hor_Slider* slider_y = new Fl_Hor_Slider(x, y+80, 220, 20, "Y");
	Fl_Hor_Slider* slider_w = new Fl_Hor_Slider(x, y+130, 220, 20, "Width");
	Fl_Hor_Slider* slider_h = new Fl_Hor_Slider(x, y+180, 220, 20, "Height");

	label->labelfont(FL_BOLD);
	slider_x->bounds(0, window.get_gl_window()->pixel_w());
	slider_y->bounds(0, window.get_gl_window()->pixel_h());
	slider_w->bounds(0, window.get_gl_window()->pixel_w());
	slider_h->bounds(0, window.get_gl_window()->pixel_h());
	slider_w->value(slider_w->maximum());
	slider_h->value(slider_h->maximum());
	slider_x->callback(callback_select_scissor<0>, &window);
	slider_y->callback(callback_select_scissor<1>, &window);
	slider_w->callback(callback_select_scissor<2>, &window);
	slider_h->callback(callback_select_scissor<3>, &window);

	window.get_scroll()->add(label);
	window.get_scroll()->add(slider_x);
	window.get_scroll()->add(slider_y);
	window.get_scroll()->add(slider_w);
	window.get_scroll()->add(slider_h);
}


void spawn_alpha_control(MainWindow<GLWindow>& window, int x, int y)
{
	Fl_Box* label = new Fl_Box(x, y, 220, 30, "Alpha test");
	Fl_Menu_Button* choice = new Fl_Menu_Button(x, y+30, 220, 30, "GL_ALWAYS");
	Fl_Hor_Slider* ref = new Fl_Hor_Slider(x, y+80, 220, 20, "Threshold");

	label->labelfont(FL_BOLD);
	ref->bounds(0, 1);
	choice->add("GL_ALWAYS");
	choice->add("GL_NEVER");
	choice->add("GL_EQUAL");
	choice->add("GL_NOTEQUAL");
	choice->add("GL_LESS");
	choice->add("GL_LEQUAL");
	choice->add("GL_GREATER");
	choice->add("GL_GEQUAL");
	choice->callback(callback_select_alpha_test, &window);
	ref->callback(callback_select_alpha_ref, &window);

	window.get_scroll()->add(label);
	window.get_scroll()->add(choice);
	window.get_scroll()->add(ref);
}


void spawn_blend_control(MainWindow<GLWindow>& window, int x, int y)
{
	Fl_Box* label = new Fl_Box(x, y, 220, 30, "Blend test");
	Fl_Box* label_src = new Fl_Box(x, y+30, 220, 30, "Source factor");
	Fl_Menu_Button* choice_src = new Fl_Menu_Button(x, y+60, 220, 30, "GL_ONE");
	Fl_Box* label_dst = new Fl_Box(x, y+90, 220, 30, "Destination factor");
	Fl_Menu_Button* choice_dst = new Fl_Menu_Button(x, y+120, 220, 30, "GL_ZERO");

	label->labelfont(FL_BOLD);
	choice_src->add("GL_ZERO");
	choice_src->add("GL_ONE");
	choice_src->add("GL_DST_COLOR");
	choice_src->add("GL_ONE_MINUS_DST_COLOR");
	choice_src->add("GL_SRC_ALPHA");
	choice_src->add("GL_ONE_MINUS_SRC_ALPHA");
	choice_src->add("GL_DST_ALPHA");
	choice_src->add("GL_ONE_MINUS_DST_ALPHA");
	choice_src->add("GL_SRC_ALPHA_SATURATE");
	choice_dst->add("GL_ZERO");
	choice_dst->add("GL_ONE");
	choice_dst->add("GL_SRC_COLOR");
	choice_dst->add("GL_ONE_MINUS_SRC_COLOR");
	choice_dst->add("GL_SRC_ALPHA");
	choice_dst->add("GL_ONE_MINUS_SRC_ALPHA");
	choice_dst->add("GL_DST_ALPHA");
	choice_dst->add("GL_ONE_MINUS_DST_ALPHA");
	choice_src->callback(callback_select_blend_src, &window);
	choice_dst->callback(callback_select_blend_dst, &window);

	window.get_scroll()->add(label);
	window.get_scroll()->add(label_src);
	window.get_scroll()->add(choice_src);
	window.get_scroll()->add(label_dst);
	window.get_scroll()->add(choice_dst);
}


void draw()
{
	constexpr static size_t point_count = sizeof(points) / sizeof(float) / 2;

	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);

	glScissor(scissor_params[0], scissor_params[1], scissor_params[2], scissor_params[3]);
	glAlphaFunc(current_alpha_test, alpha_test_ref);
	glBlendFunc(current_blend_src, current_blend_dst);

	glBegin(current_mode);
		for (size_t i = 0; i < point_count; i++)
		{
			glColor4f(colors[4*i], colors[4*i+1], colors[4*i+2], colors[4*i+3]);
			glVertex2f(points[2*i], points[2*i+1]);
		}
	glEnd();

	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
}
