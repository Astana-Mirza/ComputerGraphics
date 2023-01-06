#include <common/utils.h>

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
	return 0;
}

