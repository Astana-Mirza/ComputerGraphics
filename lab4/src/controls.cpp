#include <controls.h>

#include <common/utils.h>
#include <glm/vec4.hpp>
#include <kb_knot_storage.h>

#include <FL/Fl_Hor_Slider.H>
#include <FL/Fl_Box.H>

namespace // callbacks
{

void callback_select_tension(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Hor_Slider* slider = static_cast<Fl_Hor_Slider*>(caller);
	KBKnotStorage::get().set_tension(static_cast<float>(slider->value()));
	window->get_gl_window()->redraw();
}


void callback_select_bias(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Hor_Slider* slider = static_cast<Fl_Hor_Slider*>(caller);
	KBKnotStorage::get().set_bias(static_cast<float>(slider->value()));
	window->get_gl_window()->redraw();
}


void callback_select_continuity(Fl_Widget* caller, void* data)
{
	MainWindow<GLWindow>* window = static_cast<MainWindow<GLWindow>*>(data);
	Fl_Hor_Slider* slider = static_cast<Fl_Hor_Slider*>(caller);
	KBKnotStorage::get().set_continuity(static_cast<float>(slider->value()));
	window->get_gl_window()->redraw();
}

}


void draw()
{
	KBKnotStorage::get().draw_spline();
}


int handle(int event, GLWindow& window)
{
	glm::vec2 pos{ static_cast<float>(Fl::event_x()) / window.pixel_w() * 2 - 1,
				   static_cast<float>(Fl::event_y()) / window.pixel_w() * -2 + 1};
	int ret = 0;
	switch (event)
	{
		case FL_PUSH:
		{
			if (Fl::event_button() != FL_LEFT_MOUSE)
			{
				KBKnotStorage::get().delete_selected();
				ret = 0;
			}
			else
			{
				ret = 1;
				if (!Fl::event_ctrl() && !KBKnotStorage::get().start_drag(pos))
				{
					KBKnotStorage::get().add_knot(pos);
					KBKnotStorage::get().start_drag(pos);
				}
			}
		}
		break;
		case FL_RELEASE:
		{
			if (Fl::event_button() != FL_LEFT_MOUSE)
			{
				return 0;
			}
			if (KBKnotStorage::get().is_dragging())
			{
				KBKnotStorage::get().stop_drag();
			}
			KBKnotStorage::get().select(pos, Fl::event_ctrl());
		}
		break;
		case FL_DRAG:
		{
			KBKnotStorage::get().drag_to(pos);
		}
		break;
		default:
			return 0;
	}
	window.redraw();
	return ret;
}


void spawn_knot_control(MainWindow<GLWindow>& window, int x, int y)
{
	Fl_Hor_Slider* slider_t = new Fl_Hor_Slider(x, y + 30, 220, 20, "Tension");
	Fl_Hor_Slider* slider_b = new Fl_Hor_Slider(x, y + 80, 220, 20, "Bias");
	Fl_Hor_Slider* slider_c = new Fl_Hor_Slider(x, y + 130, 220, 20, "Continuity");

	slider_t->bounds(-1, 1);
	slider_b->bounds(-1, 1);
	slider_c->bounds(-1, 1);
	slider_t->callback(callback_select_tension, &window);
	slider_b->callback(callback_select_bias, &window);
	slider_c->callback(callback_select_continuity, &window);

	window.get_scroll()->add(slider_t);
	window.get_scroll()->add(slider_b);
	window.get_scroll()->add(slider_c);
}
