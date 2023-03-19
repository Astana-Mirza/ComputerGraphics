#ifndef _CG_KB_KNOT_STORAGE_H
#define _CG_KB_KNOT_STORAGE_H

#include <vector>
#include <set>
#include <array>
#include <memory>
#include <glm/vec2.hpp>

struct KBKnot
{
	glm::vec2 pos;
	float tension;
	float bias;
	float continuity;
};


/// Синглтон-хранилище узлов для сплайна Кочанека-Бартельса.
class KBKnotStorage
{
public:
	static KBKnotStorage& get();

	void draw_spline();

	bool select(const glm::vec2& pos, bool group = false);
	void clear_selection();

	void add_knot(const glm::vec2& pos);
	void delete_selected();
	bool start_drag(const glm::vec2& pos);
	void drag_to(const glm::vec2& pos);
	bool is_dragging() const;
	void stop_drag();

	void set_tension(float value);
	void set_bias(float value);
	void set_continuity(float value);

private:
	KBKnotStorage() = default;
	KBKnotStorage(const KBKnotStorage&) = delete;
	KBKnotStorage(KBKnotStorage&&) = delete;

	int get_knot_at_pos(const glm::vec2& pos);

private:
	static constexpr float threshold = 0.02f;

	std::set<int> selected_knots;
	std::vector<KBKnot> knots;
	std::array<float, 3> spline_color = {0.75f, 0.11f, 0.11f};
	std::array<float, 3> selection_color = {0.11f, 0.11f, 0.75f};
	bool drag = false;
};

#endif
