#include <common/utils.h>

#include <common/ui/main_window.h>
#include <common/ui/controls.h>

#include <FL/Fl_Box.h>
#include <cstdlib>
#include <ctime>

namespace
{

static constexpr GLenum modes[] = {
	  GL_POINTS
	, GL_LINES
	, GL_LINE_STRIP
	, GL_LINE_LOOP
	, GL_TRIANGLES
	, GL_TRIANGLE_STRIP
	, GL_TRIANGLE_FAN
/*	, GL_QUADS			// removed in OpenGL >=3.1
	, GL_QUAD_STRIP
	, GL_POLYGON*/
};

} // namespace

int main()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	MainWindow<GLWindow> window(800, 600, "Computer Graphics");

	spawn_background_control(window);

	window.show();
	return Fl::run();
}
