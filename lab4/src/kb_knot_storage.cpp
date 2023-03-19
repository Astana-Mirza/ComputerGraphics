#include <kb_knot_storage.h>
#include <common/utils.h>

KBKnotStorage& KBKnotStorage::get()
{
	static KBKnotStorage instance;
	return instance;
}


void KBKnotStorage::draw_spline()
{
	glPointSize(5);
	glBegin(GL_POINTS);
		for (int i = 0; i < knots.size(); i++)
		{
			if (selected_knots.find(i) != selected_knots.end())
			{
				glColor3f(selection_color[0], selection_color[1], selection_color[2]);
			}
			else
			{
				glColor3f(spline_color[0], spline_color[1], spline_color[2]);
			}
			glVertex2f(knots[i].pos.x, knots[i].pos.y);
		}
	glEnd();
	if (knots.size() < 2)
	{
		return;
	}
	std::vector<std::pair<glm::vec2, glm::vec2>> derivatives(knots.size());
	for (int i = 1; i < knots.size()-1; i++)			// расчёт значений производных
	{													// на основе параметров tension, bias, continuity
		std::pair<glm::vec2, glm::vec2> knot_der;
		glm::vec2 g1 = (knots[i].pos - knots[i-1].pos) * (1 + knots[i].bias);
		glm::vec2 g2 = (knots[i+1].pos - knots[i].pos) * (1 - knots[i].bias);
		glm::vec2 g3 = g2 - g1;
		knot_der.first  = (1 - knots[i].tension) * (g1 + 0.5f * g3 * (1 + knots[i].continuity));
		knot_der.second = (1 - knots[i].tension) * (g1 + 0.5f * g3 * (1 - knots[i].continuity));
		derivatives[i] = knot_der;
	}
	// обработка отдельного случая - для первой и последней точек
	derivatives[0].first  = knots[1].pos - knots[0].pos;
	derivatives[0].second = (1.5f * (knots[1].pos - knots[0].pos) - 0.5f * derivatives[1].first) * (1 - knots[0].tension);
	int i = knots.size() - 1;
	derivatives[i].first  = (1.5f * (knots[i].pos - knots[i-1].pos) - 0.5f * derivatives[i-1].second) * (1 - knots[i].tension);
	derivatives[i].second = knots[i].pos - knots[i-1].pos;

	glBegin(GL_LINE_STRIP);
		glColor3f(spline_color[0], spline_color[1], spline_color[2]);
		for (int i = 0; i < knots.size()-1; i++)
		{
			float step = 1.f / 100;
			float t = 0.0f;
			for (int j = 0; j < 100; j++)
			{
				glm::vec2 point = knots[i].pos * (2*t*t*t - 3*t*t*t + 1)
					            + derivatives[i].first * (t*t*t - 2*t*t + t)
					            + knots[i+1].pos * (-2*t*t*t + 3*t*t)
					            + derivatives[i+1].second * (t*t*t - t*t);
				t += step;
				glVertex2f(point.x, point.y);
			}
		}
	glEnd();
}


bool KBKnotStorage::select(const glm::vec2& pos, bool group)
{
	if (drag)
	{
		return false;
	}
	int knot_num = get_knot_at_pos(pos);
	if (!group)
	{
		selected_knots.clear();
	}
	if (-1 == knot_num)
	{
		return false;
	}
	if (group && selected_knots.find(knot_num) != selected_knots.end())
	{
		selected_knots.erase(knot_num);
		return !selected_knots.empty();
	}
	selected_knots.insert(knot_num);
	return true;
}


void KBKnotStorage::clear_selection()
{
	drag = false;
	selected_knots.clear();
}


void KBKnotStorage::add_knot(const glm::vec2& pos)
{
	knots.emplace_back(KBKnot{ pos, 0, 0, 0 });
}


void KBKnotStorage::delete_selected()
{
	for (auto iter = selected_knots.rbegin(); iter != selected_knots.rend(); ++iter)
	{
		knots.erase(knots.begin() + *iter);
	}
	clear_selection();
}


bool KBKnotStorage::start_drag(const glm::vec2& pos)
{
	int knot_num = get_knot_at_pos(pos);
	if (-1 == knot_num)
	{
		return false;
	}
	selected_knots.clear();
	selected_knots.insert(knot_num);
	drag = true;
	return true;
}


void KBKnotStorage::drag_to(const glm::vec2& pos)
{
	if (drag && selected_knots.size() == 1)
	{
		knots[*selected_knots.begin()].pos = pos;
	}
}


bool KBKnotStorage::is_dragging() const
{
	return drag;
}


void KBKnotStorage::stop_drag()
{
	drag = false;
}


void KBKnotStorage::set_tension(float value)
{
	for (int i : selected_knots)
	{
		knots[i].tension = value;
	}
}


void KBKnotStorage::set_bias(float value)
{
	for (int i : selected_knots)
	{
		knots[i].bias = value;
	}
}


void KBKnotStorage::set_continuity(float value)
{
	for (int i : selected_knots)
	{
		knots[i].continuity = value;
	}
}


int KBKnotStorage::get_knot_at_pos(const glm::vec2& pos)
{
	for (int i = 0; i < knots.size(); i++)
	{
		if (knots[i].pos.x - threshold < pos.x && knots[i].pos.x + threshold > pos.x
		 && knots[i].pos.y - threshold < pos.y && knots[i].pos.y + threshold > pos.y)
		{
			return i;
		}
	}
	return -1;
}
